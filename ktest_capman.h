#include <stdio.h>
#include <string.h>
#ifndef KLEE
#include <curses.h>
#endif
#include <assert.h>

#include "klee.h"

#if defined(KLEE) || defined(NUKLEAR)
#define SPRINTF(args...)
#define STRCAT(args...)
#else
#define SPRINTF sprintf
#define STRCAT strcat
#endif

#define SERVERIP "127.0.0.1"
#define PORT 20013

#define MAXMSGLEN 1024
#define STATUS_BAR 70 

#define C_WL_ORIG '#'
#define C_WL 'X'
#define C_SP ' '
#define C_FD '.'
#define C_PW 'o'
#define C_BM 'B'
#define CP_WL "##"
#define CP_SP "  "
#define CP_FD " ."
#define CP_PW " o"
#define CP_PL " P"
#define CP_BM " B"
#define CP_PL_L1 " >"
#define CP_PL_R1 " <"
#define CP_PL_U1 "\\/"
#define CP_PL_D1 "/\\"
#define CP_PL_L2 " -"
#define CP_PL_R2 " -"
#define CP_PL_U2 " |"
#define CP_PL_D2 " |"
#define CP_GH " A B C D"
#define CP_WIDTH 2

// #define ATTR_PL A_STANDOUT
#define ATTR_PL A_NORMAL
#define ATTR_GH1 A_NORMAL
#define ATTR_GH2 A_STANDOUT

enum {  LEFT, RIGHT, UP, DOWN };

enum {TYPE_1, TYPE_2};

#define MOVESPEED 200000 /* Sleep time between timesteps in usec */
#define POWERUPTIME 25;

// 004.map
//#define MAPFILE "capman/004.map" // 25x35
//#define MAP_ROW 25
//#define MAP_COL 35

// 005.map
#define MAPFILE "maps/005.map" // 7x9
#define MAP_ROW 7 
#define MAP_COL 9

// 006.map
//#define MAPFILE "capman/006.map" // 3x3
//#define MAP_ROW 3 
//#define MAP_COL 3

#define NUMGHOSTS 0

#define BOMBKEY 'b'
#define BLASTRADIUS 1
#define BOMB_DURATION 3 // how long the bomb animation lasts

struct point { // Test structs in Klee
  int x;
  int y;
};

// gamestatus should contain current position as well
struct gamestatus {
  int clock;
  int powerup; // Steps left with powerup active
  struct point bomb_position;
  int bomb_clock;
  //int score; // food eaten

};

void showstring(char *);
void loadmap(char *, char *);
void printmap(char *);
void updategpos(struct point *, char *);
char maploc(char *, int, int);
void updatemap(char *, struct point, struct point *, struct gamestatus *);
int foodleft(char *);
char *cp_gh(int);
int caughtbyghosts(struct point, struct point *);
void moveghosts(struct point, struct point *, int *, char *, struct gamestatus);
int canmove(struct point *, char *, int);
void setbomb(struct point *loc, struct gamestatus *gs, int bomb_timer);
void resetbomb(struct gamestatus *gs);
void makemove(struct point *, char *, int);
void makenextmove(struct point *, struct point *, char *, int);
void initializegamestatus(struct gamestatus *);
void updategamestatus(struct gamestatus *);
struct point ghoststartpoint(int);
void initializeghosts(struct point *);
int invalidposition(struct point *);

void showmap(char *m, WINDOW *);
void drawplayer(struct point, WINDOW *, struct gamestatus, int);
void drawghosts(struct point *, WINDOW *, struct gamestatus);

void showstring(char *p) {
  uint i;
  for (i=0; i<strlen(p); i++) {
    printf("%d\t%c\n",i,p[i]);
  }
}

void loadmap(char *p, char *map) {
  FILE *in;

  in = fopen(p,"r");
  if (in == NULL) {
    perror("Error opening map file");
    exit(1);
  }

  char row[MAP_COL+2]; // +1 for the newline, +1 for the \0
  int rlc, clc;
  for (rlc = 0; rlc < MAP_ROW; rlc++) {
    bzero(row,MAP_COL+2);
    fgets(row,MAP_COL+2,in);
    for (clc = 0; clc < MAP_COL; clc++) {
      char c = row[clc];
      char tmp = c;
      switch (c) {
      case C_WL_ORIG:
        tmp = C_WL;
      case C_SP:
      case C_FD:
      case C_PW:
        map[rlc*MAP_COL+clc] = tmp;
        break;
      default:
        endwin();
        perror("Malformed map");
        exit(1);
      }
    }
  }
}

void printmap(char *m) {
  int rlc, clc;
  for (rlc = 0; rlc < MAP_ROW; rlc++) {
    for (clc = 0; clc < MAP_COL; clc++) {
      char c = m[rlc*MAP_COL+clc];
      switch (c) {
      case C_WL:  printf(CP_WL);  break;
      case C_SP:  printf(CP_SP);  break;
      case C_FD:  printf(CP_FD);  break;
      default:
          endwin();
          perror("Unknown character found in map");
          exit(1);
      }
    }
    printf("\n");
  }
}

void updategpos(struct point *ghosts, char *gstr) {
  char *p = gstr;
  int g,i;
	p += 2; // advance past 'G '
  for (g=0; g<NUMGHOSTS; g++) {
    sscanf(p,"%d %d",&(ghosts[g].x),&(ghosts[g].y));
    if (g != NUMGHOSTS-1) {
      for (i=0; i<2; i++) {
        p = strchr(p,' ');
        if (p == NULL || (p+1 == '\0')) {
          endwin();
          perror("malformed ghost position string");
          printf("gstr was '%s'\n",gstr);
          exit(1);
        }
        p++;
      }
    }
  }
}

void showmap(char *m, WINDOW *win) {
  int rlc, clc;
  for (rlc = 0; rlc < MAP_ROW; rlc++) {
    for (clc = 0; clc < MAP_COL; clc++) {
      char c = m[rlc*MAP_COL+clc];
      switch (c) {
      case C_WL:
        wmove(win,rlc,clc*CP_WIDTH);
        waddstr(win,CP_WL);
        break;
      case C_SP:
        wmove(win,rlc,clc*CP_WIDTH);
        waddstr(win,CP_SP);
        break;
      case C_FD:
        wmove(win,rlc,clc*CP_WIDTH);
        waddstr(win,CP_FD);
        break;
      case C_PW:
        wmove(win,rlc,clc*CP_WIDTH);
        waddstr(win,CP_PW);
        break;
      case C_BM:
        wmove(win,rlc,clc*CP_WIDTH);
        waddstr(win,CP_BM);
        break;
      default:
        endwin();
        perror("Unknown character found in map");
        exit(1);
      }
    }
  }
  wmove(win,MAP_ROW,0);
  for (clc = 0; clc < MAP_COL*CP_WIDTH; clc++) {
    waddch(win,' ');
  }
  wmove(win,MAP_ROW+1,0);
  for (clc = 0; clc < MAP_COL*CP_WIDTH; clc++) {
    waddch(win,' ');
  }
}

void drawplayer(struct point loc, WINDOW *win, struct gamestatus gs, int dir) {
  attron(ATTR_PL);
  wmove(win,loc.y,loc.x*CP_WIDTH);
  if (gs.clock%2) {
    switch (dir) {
    case LEFT:
      waddstr(win,CP_PL_L1);
      break;
    case RIGHT:
      waddstr(win,CP_PL_R1);
      break;
    case UP:
      waddstr(win,CP_PL_U1);
      break;
    case DOWN:
      waddstr(win,CP_PL_D1);
      break;
    default:
      endwin();
      perror("invalid direction");
      exit(1);
    }
  } else {
    switch (dir) {
    case LEFT:
      waddstr(win,CP_PL_L2);
      break;
    case RIGHT:
      waddstr(win,CP_PL_R2);
      break;
    case UP:
      waddstr(win,CP_PL_U2);
      break;
    case DOWN:
      waddstr(win,CP_PL_D2);
      break;
    default:
      endwin();
      perror("invalid direction");
      exit(1);
    }
  }
  attroff(ATTR_PL);
}

void drawghosts(struct point *ghosts, WINDOW *win, struct gamestatus gs) {
  if (gs.powerup == 0 || (gs.clock%2)) {
    attron(ATTR_GH1);
  } else {
    attron(ATTR_GH2);
  }
  int g;
  for (g=NUMGHOSTS-1; g>=0; g--) {
    wmove(win,ghosts[g].y,ghosts[g].x*CP_WIDTH);
    //waddstr(win,cp_gh(g)); // malloc
  }
  if (gs.powerup == 0 || (gs.clock%2)) {
    attroff(ATTR_GH1);
  } else {
    attroff(ATTR_GH2);
  }
}

void drawbombs(WINDOW *win, struct gamestatus* gs)
{
  // Draw Bomb
  if (gs->bomb_clock > gs->clock)
  {
    wmove(win,gs->bomb_position.y,gs->bomb_position.x*CP_WIDTH);
    waddstr(win, "~@");
  }
  else if (gs->bomb_clock >= 0 && gs->bomb_clock > (gs->clock - BOMB_DURATION))
  {
    if ((gs->clock - gs->bomb_clock + 1)%2) {
      int rad_l = (gs->bomb_position.x - BLASTRADIUS)*CP_WIDTH;
      int rad_r = (gs->bomb_position.x + BLASTRADIUS)*CP_WIDTH;
      int rad_t = gs->bomb_position.y - BLASTRADIUS;
      int rad_b = gs->bomb_position.y + BLASTRADIUS;
      rad_l = rad_l >= 0 ? rad_l : 0;
      rad_r = rad_r <= MAP_COL*CP_WIDTH ? rad_r: MAP_COL*CP_WIDTH;
      rad_t = rad_t >= 0 ? rad_t : 0;
      rad_b = rad_b <= MAP_ROW ? rad_b : MAP_ROW;

      int i,j;
      for (i=rad_l; i<=rad_r; i++)
      {
        for (j=rad_t; j<=rad_b; j++)
        {
          wmove(win,j,i);
          waddstr(win, "%%");
        }
      }
    }
  }
}
 
char maploc(char *map, int x, int y) {
  return map[y*MAP_COL+x];
}

void updatemap(char *map, struct point loc, struct point *ghosts, struct gamestatus *gs) {
  int g;
  // Eat food
  if (maploc(map,loc.x,loc.y) == C_FD) {
    map[loc.y*MAP_COL+loc.x] = C_SP;
  }

  // Eat a powerup
  if (maploc(map,loc.x,loc.y) == C_PW) {
    map[loc.y*MAP_COL+loc.x] = C_SP;
    if (gs != NULL) {
      gs->powerup += POWERUPTIME;
    }
  }

  // Eat a ghost
  if (gs != NULL) {
    if (gs->powerup > 0) {
      for (g=0; g<NUMGHOSTS; g++) {
        if (loc.x == ghosts[g].x && loc.y == ghosts[g].y) {
          // Sent the ghost back home
          ghosts[g] = ghoststartpoint(g);
        }
      }
    }
  }

}

int foodleft(char *map) {
  int total = 0;
  int rlc, clc;
  for (rlc = 0; rlc < MAP_ROW; rlc++) {
    for (clc = 0; clc < MAP_COL; clc++) {
      if (maploc(map,clc,rlc) == C_FD) {
	total++;
      }
    }
  }
  return total;
}

char *cp_gh(int g) {
  char *out = (char*)malloc((CP_WIDTH+1)*sizeof(char));
  if (out == NULL) {
    endwin();
    perror("failed to malloc");
    exit(1);
  }
  int i;
  assert (0 <= g && g < NUMGHOSTS);
  for (i=0; i<CP_WIDTH; i++) {
    out[i] = CP_GH[g*CP_WIDTH+i];
    out[i] = 'a'; 
  }
  out[CP_WIDTH] = '\0'; // TODO: Do this more gracefully
  return out;
}

int caughtbyghosts(struct point loc, struct point *ghosts) {
  //return 0; // Temporary short-circuit, for testing
  int g;
  for (g=0; g<NUMGHOSTS; g++) {
    if (loc.x == ghosts[g].x && loc.y == ghosts[g].y) {
      return 1;
    }
  }
  return 0;
}

void moveghosts(struct point curr, struct point *ghosts, int *ghostdir, char *map, struct gamestatus gs) {
  int rankof_left, rankof_right, rankof_up, rankof_down;
  int front, leftside, rightside, back;
  int rankof_front, rankof_leftside, rankof_rightside, rankof_back;
  int g;

  for (g=0; g<NUMGHOSTS; g++) {

    int xd, yd;
    xd = ghosts[g].x - curr.x;
    yd = ghosts[g].y - curr.y;
    if (abs(xd) > abs(yd)) {
      // Move horizontally
      if (xd > 0) {
        rankof_left = 1;
        rankof_right = 4;
      } else {
        rankof_right = 1;
        rankof_left = 4;
      }
      if (yd > 0) {
        rankof_up = 2;
        rankof_down = 3;
      } else {
        rankof_down = 2;
        rankof_up = 3;
      }
    } else {
      // Move vertically
      if (yd > 0) {
        rankof_up = 1;
        rankof_down = 4;
      } else {
        rankof_down = 1;
        rankof_up = 4;
      }
      if (xd > 0) {
        rankof_left = 2;
        rankof_right = 3;
      } else {
        rankof_right = 2;
        rankof_left = 3;
      }
    }

    // Run away from the player if he's got a powerup
    if (gs.powerup > 0) {
      rankof_left = 5 - rankof_left;
      rankof_right = 5 - rankof_right;
      rankof_up = 5 - rankof_up;
      rankof_down = 5 - rankof_down;
    }

    switch(ghostdir[g]) {
    case LEFT:
      front = LEFT;
      leftside = DOWN;
      rightside = UP;
      back = RIGHT;
      rankof_front = rankof_left;
      rankof_leftside = rankof_down;
      rankof_rightside = rankof_up;
      rankof_back = rankof_right;
      break;
    case RIGHT:
      front = RIGHT;
      leftside = UP;
      rightside = DOWN;
      back = LEFT;
      rankof_front = rankof_right;
      rankof_leftside = rankof_up;
      rankof_rightside = rankof_down;
      rankof_back = rankof_left;
      break;
    case UP:
      front = UP;
      leftside = LEFT;
      rightside = RIGHT;
      back = DOWN;
      rankof_front = rankof_up;
      rankof_leftside = rankof_left;
      rankof_rightside = rankof_right;
      rankof_back = rankof_down;
      break;
    case DOWN:
      front = DOWN;
      leftside = RIGHT;
      rightside = LEFT;
      back = UP;
      rankof_front = rankof_down;
      rankof_leftside = rankof_right;
      rankof_rightside = rankof_left;
      rankof_back = rankof_up;
      break;
    default:
      endwin();
      perror("Invalid ghost direction");
      exit(1);
    }

    if (canmove(ghosts+g,map,front)) {
      if (canmove(ghosts+g,map,leftside)) {
        if (canmove(ghosts+g,map,rightside)) {
          // Can move: front, left, right
          if (rankof_front < rankof_leftside && rankof_front < rankof_rightside) {
            makemove(ghosts+g,map,front);
          } else if (rankof_leftside < rankof_front && rankof_leftside < rankof_rightside) {
            makemove(ghosts+g,map,leftside);
            ghostdir[g] = leftside;
          } else {
            makemove(ghosts+g,map,rightside);
            ghostdir[g] = rightside;
          }
        } else {
          // Can move: front, left
          if (rankof_front < rankof_leftside) {
            makemove(ghosts+g,map,front);
          } else {
            makemove(ghosts+g,map,leftside);
            ghostdir[g] = leftside;
          }
        }
      } else {
        if (canmove(ghosts+g,map,rightside)) {
          // Can move: front, right
          if (rankof_front < rankof_rightside) {
            makemove(ghosts+g,map,front);
          } else {
            makemove(ghosts+g,map,rightside);
            ghostdir[g] = rightside;
          }
        } else {
          // Can move: front
          makemove(ghosts+g,map,front);
        }
      }
    } else {
      if (canmove(ghosts+g,map,leftside)) {
        if (canmove(ghosts+g,map,rightside)) {
          // Can move: left, right
          if (rankof_leftside < rankof_rightside) {
            makemove(ghosts+g,map,leftside);
            ghostdir[g] = leftside;
          } else {
            makemove(ghosts+g,map,rightside);
            ghostdir[g] = rightside;
          }
        } else {
          // Can move: left
          makemove(ghosts+g,map,leftside);
          ghostdir[g] = leftside;
        }
      } else {
        if (canmove(ghosts+g,map,rightside)) {
          // Can move: right
          makemove(ghosts+g,map,rightside);
          ghostdir[g] = rightside;
        } else {
          // Can move: only backwards (if that)
          makemove(ghosts+g,map,back);
          ghostdir[g] = rightside;
        }
      }
    }
  }
}

int canmove(struct point *loc, char *map, int move) {
  switch (move) {
  case LEFT:
    if (maploc(map,(MAP_COL+loc->x-1)%MAP_COL,loc->y) != C_WL) {
      return 1;
    }
    break;
  case RIGHT:
    if (maploc(map,(loc->x+1)%MAP_COL,loc->y) != C_WL) {
      return 1;
    }
    break;
  case UP:
    if (maploc(map,loc->x,(MAP_ROW+loc->y-1)%MAP_ROW) != C_WL) {
      return 1;
    }
    break;
  case DOWN:
    if (maploc(map,loc->x,(loc->y+1)%MAP_ROW) != C_WL) {
      return 1;
    }
    break;
  default:
    endwin();
    perror("Unknown move");
    exit(1);
  }
  return 0;
}  

void setbomb(struct point *loc, struct gamestatus *gs, int bomb_timer)
{
  gs->bomb_position.x = loc->x;
  gs->bomb_position.y = loc->y;
  gs->bomb_clock = gs->clock + bomb_timer;
}

void resetbomb(struct gamestatus *gs)
{
  gs->bomb_position.x = -1;
  gs->bomb_position.y = -1;
  gs->bomb_clock = -BOMB_DURATION;
}

void makemove(struct point *loc, char *map, int move) {

  if (invalidposition(loc))
  {
    perror("Invalid position.");
    exit(1);
  }

  if (!canmove(loc,map,move)) {
    return;
  }

  switch (move) {
  case LEFT:
    loc->x = (MAP_COL+loc->x-1)%MAP_COL;
    break;
  case RIGHT:
    loc->x = (loc->x+1)%MAP_COL;
    break;
  case UP:
    loc->y = (MAP_ROW+loc->y-1)%MAP_ROW;
    break;
  case DOWN:
    loc->y = (loc->y+1)%MAP_ROW;
    break;
  default:
    endwin();
    perror("Unknown move");
    exit(1);
  }
}

void initializegamestatus(struct gamestatus *gs) {
  if (gs != NULL) {
    gs->clock = 0;
    gs->powerup = 0;
    resetbomb(gs);
    //gs->score = 0;
  }
}

void updategamestatus(struct gamestatus *gs) {
  if (gs != NULL) {
    //gs->clock = 1 - gs->clock;
    gs->clock++;
    if (gs->powerup > 0) {
      (gs->powerup)--;
    }
  }
}

/*
struct point ghoststartpoint(int g) {
  struct point out;
  out.x = 17; // Unless otherwise defined, start ghosts here
  out.y = 12;
  
  switch (g) {
  case 0:
    out.y = 11;
    break;
  case 1:
    out.x = 16;
    break;
  case 3:
    out.x = 18;
    break;
  }
  return out;
}
*/

struct point ghoststartpoint(int g) {
  struct point out;
  out.x = MAP_COL/2; // Unless otherwise defined, start ghosts here
  out.y = MAP_ROW/2;
  
  switch (g) {
  case 0:
    out.y--;
    break;
  case 1:
    out.x--;
    break;
  case 3:
    out.y++;
  case 4:
    out.x++;
    break;
  }
  return out;
}

void initializeghosts(struct point *ghosts) {
  int g;
  struct point gpoint;
  for (g=0; g<NUMGHOSTS; g++) {
    gpoint = ghoststartpoint(g);
    ghosts[g] = gpoint;
  }
}

int invalidposition(struct point *loc)
{
  if (loc->x < 0 || loc->x > MAP_COL ||
      loc->y < 0 || loc->y > MAP_ROW)
  {
    return 1;
  }
  return 0;
}

// TODO: A player can skip completely over a ghost if they are heading toward each other and swap positions in consecutive turns
