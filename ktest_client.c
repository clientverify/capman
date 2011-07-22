
#include <arpa/inet.h>
#define NCURSES_NOMACROS
#include <curses.h>
#include <getopt.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "ktest_capman.h"

#define VARIABLE_BOMB_TIMER 

#define BOMB_TIMER_MIN 3
#define BOMB_TIMER_MAX 15

#if defined(KLEE) || defined(NUKLEAR)

	#include "ktest_curses.h"

	#define KEY_DOWN        0402            /* down-arrow key */
	#define KEY_UP          0403            /* up-arrow key */
	#define KEY_LEFT        0404            /* left-arrow key */
	#define KEY_RIGHT       0405            /* right-arrow key */

#endif

#ifdef NUKLEAR 

	int nuklear_checkpoint(int x) { return x-1;}

	int getinput() {
		int c;
		klee_nuklear_make_symbolic(&c, "input");
		return c;
	}

	void setbombtimer(int *bomb_timer) {
#if defined(VARIABLE_BOMB_TIMER)
		klee_nuklear_make_symbolic(bomb_timer, "bomb_timer");
		if (*bomb_timer > BOMB_TIMER_MAX || *bomb_timer < BOMB_TIMER_MIN) exit(1);
#else
		*bomb_timer = BOMB_TIMER_MAX;
#endif
	}

#elif KLEE 

	int getinput() { return -1; }
	void setbombtimer(int *bomb_timer) { return; }

#else

	int getinput() { return getch(); }

	void setbombtimer(int *bomb_timer) {
#if defined(VARIABLE_BOMB_TIMER)
		int r = rand();
		*bomb_timer = (r % ((BOMB_TIMER_MAX+1)-BOMB_TIMER_MIN)) + BOMB_TIMER_MIN;
#else
		*bomb_timer = BOMB_TIMER_MAX;
#endif
	}

#endif


int main(int argc, char* argv[]) {
#if defined(KLEE) || defined(NUKLEAR)
  WINDOW *   stdscr = NULL;
  WINDOW *   curscr = NULL;
#endif

	// Symbolic variables
  char map[MAP_ROW*MAP_COL];
  struct gamestatus status;
  struct point curr;
  int c;
  int curr_dir;
  struct point ghosts[NUMGHOSTS];
  char messagetype;
  int bomb_timer = 0;

	// Concrete variables
	int sock;
  char msg[MAXMSGLEN];
  char resp[MAXMSGLEN];
  int status_width = STATUS_BAR; 
  char statusbar[status_width];
  char scorebar[status_width];
  char bombbar[status_width];
  char tempstr[status_width];

#if defined(KLEE)
  void** klee_ptr_copy;

  #undef  KLEE_XX_PTR
  #undef  KLEE_XX_VAR
  #define KLEE_XX_PTR(x) + 1
  #define KLEE_XX_VAR(x) + 1
  // compute number of variables names
  int klee_ptr_copy_count = 0
  #include "klee_state.inc"  
  ;

  // allocate memory for symbolic pointers
  klee_ptr_copy = (void**) malloc(sizeof(void*) * klee_ptr_copy_count);

  // Make symbolic memory
  #undef KLEE_XX_VAR
  #undef KLEE_XX_PTR
  #define KLEE_XX_PTR KLEE_MAKE_SYMBOLIC_PTR
  #define KLEE_XX_VAR KLEE_MAKE_SYMBOLIC_VAR
  #include "klee_state.inc"

  // Copy symbolic memory
  #undef  KLEE_XX_PTR
  #undef  KLEE_XX_VAR
  #define KLEE_XX_PTR KLEE_COPY_PTR
  #define KLEE_XX_VAR KLEE_COPY_VAR
  #include "klee_state.inc"

  klee_make_symbolic_unknown_size( &c , "input");

#if defined(VARIABLE_BOMB_TIMER)
	klee_make_symbolic_unknown_size( &bomb_timer, "bomb_timer");

	if (bomb_timer > BOMB_TIMER_MAX 
			|| bomb_timer < BOMB_TIMER_MIN) exit(1);
#else
	bomb_timer = BOMB_TIMER_MAX;
#endif

#endif

#if !defined(KLEE)

  int port = PORT;
  int random_movement = 0;
  int random_bomb = 0;
  int human_player = 0;
  char *input_string = NULL;
  int iteration = 0;

  char optc;
  while((optc = getopt(argc, argv, "p:rbs:h")) != -1) {
    switch (optc) {
    case 'p':
      port = atoi(optarg);
      break;
    case 'r':
      random_movement = 1;
      break;
    case 'b':
      random_bomb = 1;
      break;
    case 's':
      input_string = malloc(strlen(optarg));
      strcpy(input_string, optarg);
      break;
    case 'h':
      human_player = 1;
      break;
    }
  }

  curr_dir = RIGHT;
  loadmap(MAPFILE,map);
  curr.x = curr.y = 1; // Make starting positions defined constants?
  initializegamestatus(&status);
  initializeghosts(ghosts);

  initscr();
  cbreak();
  noecho();
  keypad(stdscr,TRUE);
  nodelay(stdscr, TRUE);

  bzero(resp,MAXMSGLEN);
  bzero(statusbar,status_width);
  bzero(scorebar,status_width);
  bzero(bombbar,status_width);
  bzero(scorebar,status_width);

  sock = socket(PF_INET, SOCK_STREAM, 0);
 
  if (sock == -1) {
    perror("Error creating client socket");
    exit(-1);
  }

  struct sockaddr_in sa;
  bzero(&sa, sizeof(sa));
 
  sa.sin_family = AF_INET;
  sa.sin_port = htons(port);
  int addr = inet_pton(AF_INET, SERVERIP, (void *) &sa.sin_addr);
 
  if (addr < 0) {
    perror("error: first parameter is not a valid address family");
    exit(-1);
  } else if (0 == addr) {
    perror("char string (second parameter does not contain valid ipaddress");
    exit(-1);
  }
 
  if (connect(sock,(struct sockaddr*) &sa, sizeof(sa)) == -1) {
    printf("Connection to server failed\n");
    endwin();
    exit(-1);
  }
#endif

#if defined(NUKLEAR)
	human_player = 1;
#endif 

  while (1) { //foodleft(map)) {

		// begin bounds checking
		if (status.powerup < 0) break;
		if (caughtbyghosts(curr, ghosts)) break;
		if (invalidposition(&curr)) break;
		
		int g;
		for (g=0; g<NUMGHOSTS; g++)
		  if(invalidposition(&(ghosts[g]))) exit(1);
		// end bounds checking

		updategamestatus(&status);

#if !defined(KLEE)
		// Draw the map
		showmap(map,stdscr);
		drawplayer(curr,stdscr,status,curr_dir);
		drawghosts(ghosts,stdscr,status);
		drawbombs(stdscr, &status);

		// Status lines:
		wmove(stdscr,MAP_ROW,0);
		SPRINTF(scorebar,"Food left: %d (%d,%d)",foodleft(map),curr.x,curr.y);
		if (status.powerup > 0) {
			SPRINTF(tempstr,", Powerup time left: %d",status.powerup);
			STRCAT(scorebar,tempstr);
		}
		waddstr(stdscr,scorebar);
		wmove(stdscr,MAP_ROW+1,0);
		waddstr(stdscr,statusbar);
		wmove(stdscr,MAP_ROW+2,0);
		waddstr(stdscr,bombbar);
		wmove(stdscr,MAP_ROW+3,0);

		wrefresh(stdscr);

		if (human_player)
			usleep(MOVESPEED);

		if (human_player) {
			c = getinput();
		} else {
			if (random_movement) {
				char inputs[4] = {'e', 's', 'd', 'f'};
				c = inputs[rand() % 4];
			} else if (input_string) {
				c = input_string[(iteration++) % (strlen(input_string))];
			} else {
				endwin();
				printf("Error: please specify input type.\n");
				exit(1);
			}
			if (random_bomb) {
				if (status.bomb_clock < (status.clock - BOMB_DURATION)) {
					if ((rand() % 32) == 0)
						c = BOMBKEY;
				}
			}
		}
#endif
		

		switch (c) {
			case 'q':
				endwin();
				shutdown(sock, 2);
				close(sock);
				exit(1);
			break;
			case 'e':
			case KEY_UP:
				curr_dir = UP;
				break;
			case 'd':
			case KEY_DOWN:
				curr_dir = DOWN;
				break;
			case 's':
			case KEY_LEFT:
				curr_dir = LEFT;
				break;
			case 'f':
			case KEY_RIGHT:
				curr_dir = RIGHT;
				break;
			case BOMBKEY:
				if (status.bomb_clock < (status.clock - BOMB_DURATION)) {
					setbombtimer(&bomb_timer);
					setbomb(&curr, &status, bomb_timer);
					SPRINTF(bombbar, "BomB in... ");
				} else {exit(1);}
				break;
#ifdef CHEATS_ENABLED
			case 'x':
				curr.x = curr.x+3;
				break;
			case 'z':
				curr.x = curr.x-3;
				break;
			case 'B':
				setbomb(&curr, &status, BOMB_TIMER_MIN);
				status.bomb_position.x = 3; 
				status.bomb_position.y = 1;
				status.bomb_clock = status.clock;
				break;
#endif
			case ERR:
			//	wmove(stdscr,MAP_ROW,0);
			//	break;
			default:
				wmove(stdscr,MAP_ROW,0);
				break;
		}

		messagetype = TYPE_1;
		if (status.bomb_clock == status.clock) {
			SPRINTF(tempstr,"KABOOM!");
			STRCAT(bombbar, tempstr);
			messagetype = TYPE_2;
		} 
		else if (status.clock == status.bomb_clock + BOMB_DURATION) {
			memset(bombbar,' ',status_width);
			bombbar[status_width-1] = 0;
			resetbomb(&status);
		} else if (status.bomb_clock > status.clock) {
			// Bomb timer countdown
			SPRINTF(tempstr,"%d ", (status.bomb_clock - status.clock));
			STRCAT(bombbar, tempstr);
		}

		makemove(&curr, map, curr_dir);
		updatemap(map,curr,ghosts,&status);

#if defined(KLEE)
    {
      #undef  KLEE_XX_PTR
      #undef  KLEE_XX_VAR
      #define KLEE_XX_PTR KLEE_EQUAL_PTR
      #define KLEE_XX_VAR KLEE_EQUAL_VAR
      #include "klee_state.inc"
    }

    klee_write_constraints();
    exit(1);
#endif


		if (messagetype == TYPE_1) {
			sprintf(msg,"%d %d %d %d %d",
						curr.x, curr.y, status.powerup, -1, -1);
		} else if (messagetype == TYPE_2) {
			sprintf(msg,"%d %d %d %d %d",
						curr.x, curr.y, status.powerup, 
						status.bomb_position.x, status.bomb_position.y);
		}

		send(sock,msg,strlen(msg)+1, 0);

#if defined(NUKLEAR) 
		nuklear_checkpoint(1);
#endif

		bzero(resp,MAXMSGLEN);

		recv(sock,resp,MAXMSGLEN,0);

		updategpos(ghosts,resp);
  }

  endwin();
  shutdown(sock, 2);
  close(sock);
  printf("closing...\n");

  return 0;
}
