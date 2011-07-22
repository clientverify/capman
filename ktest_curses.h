
#ifndef __KLEE_CURSES_H
#define __KLEE_CURSES_H


//WINDOW *   stdscr = NULL;
//WINDOW *   curscr = NULL;
int	LINES;
int	COLS;
int	TABSIZE;
int COLORS;
int COLOR_PAIRS;

/*
 * This global was an undocumented feature under AIX curses.
 */
int ESCDELAY;	/* ESC expire time in milliseconds */

extern char ttytype[];	/* needed for backward compatibility */


WINDOW* initscr () { return NULL; }

int addch (const chtype v1) { return OK; }
int addchnstr (const chtype * v1,  int v2) { return OK; }
int addchstr (const chtype * v1) { return OK; }
int addnstr (const char * v1,  int v2) { return OK; }
int addstr (const char * v1) { return OK; }
int attr_get (attr_t * v1,  short * v2,  void * v3) { return OK; }
int attr_off (attr_t v1,  void * v2) { return OK; }
int attroff (NCURSES_ATTR_T v1) { return OK; }
int attr_on (attr_t v1,  void * v2) { return OK; }
int attron (NCURSES_ATTR_T v1) { return OK; }
int attr_set (attr_t v1,  short v2,  void * v3) { return OK; }
int attrset (NCURSES_ATTR_T v1) { return OK; }
int baudrate () { return OK; }
int beep  () { return OK; }
int bkgd (chtype v1) { return OK; }
int border (chtype v1,chtype v2,chtype v3,chtype v4,chtype v5,chtype v6,chtype v7,chtype v8) { return OK; }
int box (WINDOW * v1,  chtype v2,  chtype v3) { return OK; }
int cbreak () { return OK; }
int chgat (int v1,  attr_t v2,  short v3,  const void * v4) { return OK; }
int clearok (WINDOW * v1, bool v2) { return OK; }
int clear () { return OK; }
int clrtobot () { return OK; }
int clrtoeol () { return OK; }
int color_content (short v1, short* v2, short* v3, short* v4) { return OK; }
int COLOR_PAIR (int v1) { return OK; }
int color_set (short v1, void* v2) { return OK; }
int copywin (const WINDOW* v1,WINDOW* v2,int v3,int v4,int v5,int v76,int v7,int v8,int v9) { return OK; }
int curs_set (int v1) { return OK; }
int def_prog_mode () { return OK; }
int def_shell_mode () { return OK; }
int delay_output (int v1) { return OK; }
int delch () { return OK; }
int deleteln () { return OK; }
int delwin (WINDOW * v1) { return OK; }
int doupdate () { return OK; }
int echochar (const chtype v1) { return OK; }
int echo () { return OK; }
int endwin () { return OK; }
int erase () { return OK; }
int flash () { return OK; }
int flushinp () { return OK; }
int getbegx (const WINDOW * v1) { return OK; }
int getbegy (const WINDOW * v1) { return OK; }
int getch () { return OK; }
int getcurx (const WINDOW * v1) { return OK; }
int getcury (const WINDOW * v1) { return OK; }
int getmaxx (const WINDOW * v1) { return OK; }
int getmaxy (const WINDOW * v1) { return OK; }
int getnstr (char * v1,  int v2) { return OK; }
int getparx (const WINDOW * v1) { return OK; }
int getpary (const WINDOW * v1) { return OK; }
int getstr (char * v1) { return OK; }
int halfdelay (int v1) { return OK; }
int hline (chtype v1,  int v2) { return OK; }
int idlok (WINDOW * v1,  bool v2) { return OK; }
int inchnstr (chtype * v1,  int v2) { return OK; }
int inchstr (chtype * v1) { return OK; }
int init_color (short v1, short v2, short v3, short v4) { return OK; }
int init_pair (short v1, short v2, short v3) { return OK; }
int innstr (char * v1,  int v2) { return OK; }
int insch (chtype v1) { return OK; }
int insdelln (int v1) { return OK; }
int insertln () { return OK; }
int insnstr (const char * v1,  int v2) { return OK; }
int insstr (const char * v1) { return OK; }
int instr (char * v1) { return OK; }
int intrflush (WINDOW * v1, bool v2) { return OK; }
int keypad (WINDOW * v1, bool v2) { return OK; }
int leaveok (WINDOW * v1, bool v2) { return OK; }
int meta (WINDOW * v1, bool v2) { return OK; }
int move (int v1,  int v2) { return OK; }
int mvaddch (int v1,  int v2,  const chtype v3) { return OK; }
int mvaddchnstr (int v1,  int v2,  const chtype * v3,  int v4) { return OK; }
int mvaddchstr (int v1,  int v2,  const chtype * v3) { return OK; }
int mvaddnstr (int v1,  int v2,  const char * v3,  int v4) { return OK; }
int mvaddstr (int v1,  int v2,  const char * v3) { return OK; }
int mvchgat (int v1,  int v2,  int v3,  attr_t v4,  short v5,  const void * v6) { return OK; }
int mvcur (int v1, int v2, int v3, int v4) { return OK; }
int mvdelch (int v1,  int v2) { return OK; }
int mvderwin (WINDOW * v1,  int v2,  int v3) { return OK; }
int mvgetch (int v1,  int v2) { return OK; }
int mvgetnstr (int v1,  int v2,  char * v3,  int v4) { return OK; }
int mvgetstr (int v1,  int v2,  char * v3) { return OK; }
int mvhline (int v1,  int v2,  chtype v3,  int v4) { return OK; }
int mvinchnstr (int v1,  int v2,  chtype * v3,  int v4) { return OK; }
int mvinchstr (int v1,  int v2,  chtype * v3) { return OK; }
int mvinnstr (int v1,  int v2,  char * v3,  int v4) { return OK; }
int mvinsch (int v1,  int v2,  chtype v3) { return OK; }
int mvinsnstr (int v1,  int v2,  const char * v3,  int v4) { return OK; }
int mvinsstr (int v1,  int v2,  const char * v3) { return OK; }
int mvinstr (int v1,  int v2,  char * v3) { return OK; }
int mvprintw (int v1,int v2, const char * v3,...) { return OK; }
int mvscanw (int v1,int v2, NCURSES_CONST char * v3,...) { return OK; }
int mvvline (int v1,  int v2,  chtype v3,  int v4) { return OK; }
int mvwaddchnstr (WINDOW * v1,  int v2,  int v3,  const chtype * v4,  int v5) { return OK; }
int mvwaddchstr (WINDOW * v1,  int v2,  int v3,  const chtype * v4) { return OK; }
int mvwaddch (WINDOW * v1,  int v2,  int v3,  const chtype v4) { return OK; }
int mvwaddnstr (WINDOW * v1,  int v2,  int v3,  const char * v4,  int v5) { return OK; }
int mvwaddstr (WINDOW * v1,  int v2,  int v3,  const char * v4) { return OK; }
int mvwchgat (WINDOW * v1, int v2, int v3, int v4, attr_t v5, short v6, const void * v7) { return OK; }
int mvwdelch (WINDOW * v1,  int v2,  int v3) { return OK; }
int mvwgetch (WINDOW * v1,  int v2,  int v3) { return OK; }
int mvwgetnstr (WINDOW * v1,  int v2,  int v3,  char * v4,  int v5) { return OK; }
int mvwgetstr (WINDOW * v1,  int v2,  int v3,  char * v4) { return OK; }
int mvwhline (WINDOW * v1,  int v2,  int v3,  chtype v4,  int v5) { return OK; }
int mvwinchnstr (WINDOW * v1,  int v2,  int v3,  chtype * v4,  int v5) { return OK; }
int mvwinchstr (WINDOW * v1,  int v2,  int v3,  chtype * v4) { return OK; }
int mvwinnstr (WINDOW * v1,  int v2,  int v3,  char * v4,  int v5) { return OK; }
int mvwinsch (WINDOW * v1,  int v2,  int v3,  chtype v4) { return OK; }
int mvwinsnstr (WINDOW * v1,  int v2,  int v3,  const char * v4,  int v5) { return OK; }
int mvwinsstr (WINDOW * v1,  int v2,  int v3,  const char * v4) { return OK; }
int mvwinstr (WINDOW * v1,  int v2,  int v3,  char * v4) { return OK; }
int mvwin (WINDOW * v1, int v2, int v3) { return OK; }
int mvwprintw (WINDOW* v1,int v2,int v3, const char * v4,...)	{ return OK; }
int mvwscanw (WINDOW * v1,int v2,int v3, NCURSES_CONST char * v4,...)	{ return OK; }
int mvwvline (WINDOW * v1, int v2,  int v3,  chtype v4,  int v5) { return OK; }
int napms (int v1) { return OK; }
int nl () { return OK; }
int nocbreak () { return OK; }
int nodelay (WINDOW * v1, bool v2) { return OK; }
int noecho () { return OK; }
int nonl () { return OK; }
int noraw () { return OK; }
int notimeout (WINDOW * v1, bool v2) { return OK; }
int overlay (const WINDOW* v1, WINDOW * v2) { return OK; }
int overwrite (const WINDOW* v1, WINDOW * v2) { return OK; }
int pair_content (short v1, short* v2, short* v3) { return OK; }
int PAIR_NUMBER (int v1) { return OK; }
int pechochar (WINDOW * v1,  const chtype v2) { return OK; }
//int pnoutrefresh (WINDOW*,int,int,int,int,int,int) { return OK; }
//int prefresh (WINDOW *,int,int,int,int,int,int) { return OK; }
int printw (const char * v1,...) { return OK; }
int putp (const char * v1) { return OK; }
int putwin (WINDOW * v1,  FILE * v2) { return OK; }
int raw () { return OK; }
int redrawwin (WINDOW * v1) { return OK; }
int refresh () { return OK; }
int reset_prog_mode () { return OK; }
int reset_shell_mode () { return OK; }
int resetty () { return OK; }
int ripoffline (int v1,  int (*v2)(WINDOW *, int)) { return OK; }
int savetty () { return OK; }
int scanw (NCURSES_CONST char * v1,...)	{ return OK; }
int scr_dump (const char * v1) { return OK; }
int scr_init (const char * v1) { return OK; }
int scrl (int v1) { return OK; }
int scrollok (WINDOW * v1, bool v2) { return OK; }
int scroll (WINDOW * v1) { return OK; }
int scr_restore (const char * v1) { return OK; }
int scr_set (const char * v1) { return OK; }
int setscrreg (int v1, int v2) { return OK; }
int slk_attr_off (const attr_t v1,  void * v2) { return OK; }
int slk_attroff (const chtype v1) { return OK; }
int slk_attr_on (attr_t v1, void* v2) { return OK; }
int slk_attron (const chtype v1) { return OK; }
int slk_attr_set (const attr_t v1, short v2, void* v3) { return OK; }
int slk_attrset (const chtype v1) { return OK; }
int slk_clear () { return OK; }
int slk_color (short v1) { return OK; }
int slk_init (int v1) { return OK; }
int slk_noutrefresh () { return OK; }
int slk_refresh () { return OK; }
int slk_restore () { return OK; }
int slk_set (int v1, const char * v2, int v3) { return OK; }
int slk_touch () { return OK; }
int standend () { return OK; }
int standout () { return OK; }
int start_color () { return OK; }
int syncok (WINDOW * v1,  bool v2) { return OK; }
int tigetflag (NCURSES_CONST char * v1) { return OK; }
int tigetnum (NCURSES_CONST char * v1) { return OK; }
int touchline (WINDOW * v1,  int v2,  int v3) { return OK; }
int touchwin (WINDOW * v1) { return OK; }
int typeahead (int v1) { return OK; }
int ungetch (int v1) { return OK; }
int untouchwin (WINDOW * v1) { return OK; }
int vidattr (chtype v1) { return OK; }
int vidputs (chtype, int (*)(int));		
int vline (chtype v1,  int v2) { return OK; }
int vw_printw (WINDOW * v1,  const char * v2, va_list v3) { return OK; }
int vwprintw (WINDOW * v1,  const char * v2, va_list v3) { return OK; }
int vw_scanw (WINDOW * v1,  NCURSES_CONST char * v2, va_list v3) { return OK; }
int vwscanw (WINDOW * v1,  NCURSES_CONST char * v2, va_list v3) { return OK; }
int waddchnstr (WINDOW * v1, const chtype * v2, int v3) { return OK; }
int waddchstr (WINDOW * v1, const chtype * v2) { return OK; }
int waddch (WINDOW * v1,  const chtype v2) { return OK; }
int waddnstr (WINDOW * v1, const char * v2, int v3) { return OK; }
int waddstr (WINDOW * v1, const char * v2) { return OK; }
int wattr_get (WINDOW * v1,  attr_t * v2,  short * v3,  void * v4) { return OK; }
int wattr_off (WINDOW * v1,  attr_t v2,  void * v3) { return OK; }
int wattroff (WINDOW * v1,  int v2) { return OK; }
int wattr_on (WINDOW * v1,  attr_t v2,  void * v3) { return OK; }
int wattron (WINDOW * v1,  int v2) { return OK; }
int wattr_set (WINDOW * v1,  attr_t v2,  short v3,  void * v4) { return OK; }
int wattrset (WINDOW * v1,  int v2) { return OK; }
int wbkgd (WINDOW * v1,  chtype v2) { return OK; }
//int wborder (WINDOW *,chtype,chtype,chtype,chtype,chtype,chtype,chtype,chtype) { return OK; }
int wchgat (WINDOW * v1,  int v2,  attr_t v3,  short v4,  const void * v5) { return OK; }
int wclear (WINDOW * v1) { return OK; }
int wclrtobot (WINDOW * v1) { return OK; }
int wclrtoeol (WINDOW * v1) { return OK; }
int wcolor_set (WINDOW* v1, short v2, void* v3) { return OK; }
int wdelch (WINDOW * v1) { return OK; }
int wdeleteln (WINDOW * v1) { return OK; }
int wechochar (WINDOW * v1,  const chtype v2) { return OK; }
int werase (WINDOW * v1) { return OK; }
int wgetch (WINDOW * v1) { return OK; }
int wgetnstr (WINDOW * v1, char * v2, int v3) { return OK; }
int wgetstr (WINDOW * v1,  char * v2) { return OK; }
int whline (WINDOW * v1,  chtype v2,  int v3) { return OK; }
int winchnstr (WINDOW * v1,  chtype * v2,  int v3) { return OK; }
int winchstr (WINDOW * v1,  chtype * v2) { return OK; }
int winnstr (WINDOW * v1,  char * v2,  int v3) { return OK; }
int winsch (WINDOW * v1,  chtype v2) { return OK; }
int winsdelln (WINDOW * v1, int v2) { return OK; }
int winsertln (WINDOW * v1) { return OK; }
int winsnstr (WINDOW * v1,  const char * v2, int v3) { return OK; }
int winsstr (WINDOW * v1,  const char * v2) { return OK; }
int winstr (WINDOW * v1,  char * v2) { return OK; }
int wmove (WINDOW * v1, int v2, int v3) { return OK; }
int wnoutrefresh (WINDOW * v1) { return OK; }
int wprintw (WINDOW * v1, const char * v2,...) { return OK; }
int wredrawln (WINDOW * v1, int v2, int v3) { return OK; }
int wrefresh (WINDOW * v1) { return OK; }
int wscanw (WINDOW *v1, NCURSES_CONST char * v2,...) { return OK; }
int wscrl (WINDOW * v1, int v2) { return OK; }
int wsetscrreg (WINDOW * v1, int v2, int v3) { return OK; }
int wstandend (WINDOW * v1) { return OK; }
int wstandout (WINDOW * v1) { return OK; }
int wtouchln (WINDOW * v1, int v2, int v3, int v4) { return OK; }
int wvline (WINDOW * v1, chtype v2, int v3) { return OK; }

/*
extern NCURSES_EXPORT(attr_t) slk_attr ();				
extern NCURSES_EXPORT(bool) can_change_color ();			
extern NCURSES_EXPORT(bool) has_colors ();				
extern NCURSES_EXPORT(bool) has_ic ();				
extern NCURSES_EXPORT(bool) has_il ();				
extern NCURSES_EXPORT(bool) isendwin ();				
extern NCURSES_EXPORT(bool) is_linetouched (WINDOW *,int);		
extern NCURSES_EXPORT(bool) is_wintouched (WINDOW *);			
extern NCURSES_EXPORT(char) erasechar ();				
extern NCURSES_EXPORT(char) killchar ();				
extern NCURSES_EXPORT(char *) longname ();				
extern NCURSES_EXPORT(char *) slk_label (int);				
extern NCURSES_EXPORT(char *) termname ();				
extern NCURSES_EXPORT(char *) tigetstr (NCURSES_CONST char *);		
extern NCURSES_EXPORT(char *) tparm (NCURSES_CONST char *, ...);	
extern NCURSES_EXPORT(chtype) getbkgd (WINDOW *);			
extern NCURSES_EXPORT(chtype) inch ();				
extern NCURSES_EXPORT(chtype) mvinch (int, int);			
extern NCURSES_EXPORT(chtype) mvwinch (WINDOW *, int, int);			
extern NCURSES_EXPORT(chtype) termattrs ();				
extern NCURSES_EXPORT(chtype) winch (WINDOW *);				
extern NCURSES_EXPORT(NCURSES_CONST char *) keyname (int);		
extern NCURSES_EXPORT(SCREEN *) newterm (NCURSES_CONST char *,FILE *,FILE *);	
extern NCURSES_EXPORT(SCREEN *) set_term (SCREEN *);			
extern NCURSES_EXPORT() bkgdset (chtype);				
extern NCURSES_EXPORT() delscreen (SCREEN *);			
extern NCURSES_EXPORT() filter ();				
extern NCURSES_EXPORT() idcok (WINDOW *, bool);			
extern NCURSES_EXPORT() immedok (WINDOW *, bool);			
extern NCURSES_EXPORT() noqiflush ();				
extern NCURSES_EXPORT() qiflush ();				
extern NCURSES_EXPORT() timeout (int);				
extern NCURSES_EXPORT() use_env (bool);				
extern NCURSES_EXPORT() wbkgdset (WINDOW *,chtype);			
extern NCURSES_EXPORT() wcursyncup (WINDOW *);			
extern NCURSES_EXPORT() wsyncdown (WINDOW *);			
extern NCURSES_EXPORT() wsyncup (WINDOW *);				
extern NCURSES_EXPORT() wtimeout (WINDOW *,int);			
extern NCURSES_EXPORT(WINDOW *) derwin (WINDOW *,int,int,int,int);	
extern NCURSES_EXPORT(WINDOW *) dupwin (WINDOW *);			
extern NCURSES_EXPORT(WINDOW *) getwin (FILE *);			
extern NCURSES_EXPORT(WINDOW *) initscr ();				
extern NCURSES_EXPORT(WINDOW *) newpad (int,int);				
extern NCURSES_EXPORT(WINDOW *) newwin (int,int,int,int);			
extern NCURSES_EXPORT(WINDOW *) subpad (WINDOW *, int, int, int, int);	
extern NCURSES_EXPORT(WINDOW *) subwin (WINDOW *,int,int,int,int);	
*/

#endif //__KLEE_CURSES_H


