#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <curses.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <getopt.h>

#undef FALSE
#undef TRUE 

#include "ktest_capman.h"

# include <errno.h>
# include <sys/stat.h>

#define KTEST_FILE "capman.ktest"
#include "KTest.h"
#define DISABLE_GUI

KTestObject* ktest_objects = NULL;
int num_ktest_objects = -1;
int max_ktest_objects = 0;
enum { CLIENT_TO_SERVER=0, SERVER_TO_CLIENT=1 };
char* ktest_object_names[] = { "c2s", "s2c" };

static inline void ktest_check_mem() {
  if (num_ktest_objects >= max_ktest_objects) {
    max_ktest_objects = (max_ktest_objects+1)*2;
    size_t size = max_ktest_objects * sizeof(KTestObject);
    ktest_objects = (KTestObject*) realloc(ktest_objects, size);
    if (!ktest_objects) {
      fprintf(stderr, "ERROR in ktest_check_mem\n");
      exit(EXIT_FAILURE);
    }
  }
}

ssize_t ktest_write(int fd, const void *buf, size_t count) {
  int i = ++num_ktest_objects;

  ssize_t num_bytes = write(fd, buf, count);

  if (num_bytes >= 0) {
    ktest_check_mem();
    ktest_objects[i].name = ktest_object_names[SERVER_TO_CLIENT];
    ktest_objects[i].numBytes = num_bytes;
    ktest_objects[i].bytes = (unsigned char*) malloc(sizeof (unsigned char) * num_bytes);
    memcpy(ktest_objects[i].bytes, buf, num_bytes);
  } else {
    fprintf(stderr, "ERROR in ktest_write\n");
    exit(EXIT_FAILURE);
  }
  return num_bytes;
}


ssize_t ktest_read(int fd, void *buf, size_t count) {
  int i = ++num_ktest_objects;

  ssize_t num_bytes = read(fd, buf, count);

  if (num_bytes >= 0) {
    ktest_check_mem();
    ktest_objects[i].name = ktest_object_names[CLIENT_TO_SERVER];
    ktest_objects[i].numBytes = num_bytes;
    ktest_objects[i].bytes = (unsigned char*) malloc(sizeof (unsigned char) * num_bytes);
    memcpy(ktest_objects[i].bytes, buf, num_bytes);
  } else {
    fprintf(stderr, "ERROR in ktest_read\n");
    exit(EXIT_FAILURE);
  }
  return num_bytes;
}

void ktest_finish(int argc, char** argv) {
  fprintf(stdout, "Writing KTest file.\n");
  KTest ktest;
  ktest.numArgs = argc;
  ktest.args = argv;
  ktest.symArgvs = 0;
  ktest.symArgvLen = 0;
  ktest.numObjects = num_ktest_objects;
  ktest.objects = ktest_objects;
  int i;
  for (i = 0; i<num_ktest_objects; i++) {
    printf("ktest_object[%d].name = %s\n",
           i, ktest_objects[i].name);
  }
  int result = kTest_toFile(&ktest, KTEST_FILE);
  if (!result) {
    fprintf(stderr, "ERROR in ktest_finish\n");
  }
  exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[]) {

  //atexit(ktest_finish);

  int i, g;

  char map[MAP_ROW*MAP_COL];
  loadmap(MAPFILE,map);
  struct gamestatus status;
  initializegamestatus(&status);
  int curr_dir = RIGHT;
  struct point curr;
  struct point prev;
  curr.x = curr.y = 1;
  struct point ghosts[NUMGHOSTS];
  int ghostdir[NUMGHOSTS];
  initializeghosts(ghosts);
  for (g=0; g<NUMGHOSTS; g++) {
    ghostdir[g] = RIGHT;
  }

  char tempstr[MAXMSGLEN];

  char statusbar[STATUS_BAR];
  bzero(statusbar,STATUS_BAR);
  char scorebar[STATUS_BAR];
  bzero(scorebar,STATUS_BAR);

  int port = PORT;
  bool verbose = false;
  int max_rounds = 0;

  char optc;
  while((optc = getopt(argc, argv, "p:c:v")) != -1) {
    switch (optc) {
    case 'p':
      port = atoi(optarg);
      break;
    case 'v':
      verbose = true;
      break;
    case 'c':
      max_rounds = atoi(optarg);
      break;
    }
  }
  int servsock = socket(PF_INET, SOCK_STREAM, 0);
 
  if (servsock == -1) {
    perror("Error creating server socket");
    exit(-1);
  }
 
  struct sockaddr_in sa;
  bzero(&sa, sizeof(sa));
 
  sa.sin_family = AF_INET;
  sa.sin_port = htons(port);
  
  if (bind(servsock,(struct sockaddr*) &sa, sizeof(sa)) == -1) {
    perror("Error binding to server port");
    exit(-1);
  }
 
  if (listen(servsock, 10) == -1) {
    perror("Error on listen");
    exit(-1);
  }
 
  for(;;) {
    int sock = accept(servsock, NULL, NULL);
 
    if (sock < 0) {
      perror("Error accepting connection");
      exit(-1);
    }

    char msg[MAXMSGLEN];
    bzero(msg,MAXMSGLEN);
    char resp[MAXMSGLEN];
    bzero(resp,MAXMSGLEN);

    // perform read write operations ...
    //strncpy(resp,"This si my resp.\n",MAXMSGLEN-1);
    //write(sock,resp,strlen(resp));

#ifndef DISABLE_GUI 
    initscr();
#endif

    int lenread = 0;
    int round_number = 0;
    //while (foodleft(map) > 0) {
    while (1) {
      round_number++;
      if (max_rounds && round_number > max_rounds) break;
      updategamestatus(&status);

#ifndef DISABLE_GUI 
      // Draw the map
      showmap(map,stdscr);
      drawplayer(curr,stdscr,status,curr_dir);
      drawghosts(ghosts,stdscr,status);

      // Status lines:
      wmove(stdscr,MAP_ROW,0);
      sprintf(scorebar,"Food left: %d (%d,%d)",foodleft(map),curr.x,curr.y);
#endif
      if (status.powerup > 0) {
        sprintf(tempstr,", Powerup time left: %d",status.powerup);
        strcat(scorebar,tempstr);
      }
#ifndef DISABLE_GUI 
      waddstr(stdscr,scorebar);
      wmove(stdscr,MAP_ROW+1,0);
      waddstr(stdscr,statusbar);
      wmove(stdscr,MAP_ROW+2,0);
      wrefresh(stdscr);
#endif      

      // Update status normally, but then overwrite status.powerup from the client
      //updatemap(map,curr,ghosts,&status); 

      prev.x = curr.x;
      prev.y = curr.y;
      strcpy(statusbar,"");

#ifndef DUMMY_CLIENT
      lenread = ktest_read(sock,msg,MAXMSGLEN-1); // Check read results/socket status

      //printf("Recv: %s\n", msg);
      sscanf(msg,"%d %d %d %d %d",
                 &(curr.x),&(curr.y),&(status.powerup), 
                 &(status.bomb_position.x), &(status.bomb_position.y));
#endif
      updatemap(map,curr,ghosts,&status); 

      //moveghosts(curr,ghosts,ghostdir,map,status);

      bzero(resp,MAXMSGLEN);
      bzero(tempstr,MAXMSGLEN);

			sprintf(tempstr,"G ");
			strcat(resp,tempstr);

      for (g=0; g<NUMGHOSTS; g++) {
        sprintf(tempstr,"%d %d ",ghosts[g].x,ghosts[g].y);
        strcat(resp,tempstr);
      }

      ktest_write(sock,resp,strlen(resp));
    }

    shutdown(sock, 2);
    close(sock);

    ktest_finish(argc, argv);

  }
#ifndef DISABLE_GUI 
  endwin();
#endif

  return 0;
}
