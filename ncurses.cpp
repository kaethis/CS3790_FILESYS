#include <ncurses.h>


#define  SCREENX   90
#define  SCREENY   50


class Window{

  public:

	WINDOW  *win;


	int  y, x;

	int  wid, len;

	int  y_min, y_max;
	int  x_min, x_max;

	int  y_curr, x_curr;


	Window(int, int, int, int);

	void drawHeader(void);

	void drawOK(void);

	void drawCancel(void);

	void close(void);
};


Window::Window(int wid, int len, int y, int x){

	this->y = y;
	this->x = x;

	this->wid = wid;
	this ->len = len;


	win = newwin(len, wid, y, x);


	y_min = 1;
	y_max = (len-2);
	x_min = 1;
	x_max = (wid-2);

	y_curr = y_min;
	x_curr = x_min;


	box(win, 0, 0);

	wbkgd(win, COLOR_PAIR(1));


	refresh();

	wrefresh(win);
}


void Window::drawHeader(){

	move(y-1, x);

	refresh();

	attron(COLOR_PAIR(3));

	for(int i = 0; i < wid; i++)
		printw(" ");

	attroff(COLOR_PAIR(3));


	wrefresh(win);


	move(0, 0);

	refresh();
}


void Window::drawOK(){

	wmove(win, y_max, x_max-7);
	refresh();

	wattron(win, COLOR_PAIR(2));
	wprintw(win, "[ret]");		

	wattron(win, COLOR_PAIR(1));
	wprintw(win, "OK");		

	wrefresh(win);


	move(0, 0);

	refresh();
}


void Window::drawCancel(){

	wmove(win, y_max, x_min+1);
	refresh();

	wattron(win, COLOR_PAIR(2));
	wprintw(win, "[esc]");		

	wattron(win, COLOR_PAIR(1));
	wprintw(win, "CANCEL");	

	wrefresh(win);


	move(0, 0);

	refresh();
}


void Window::close(){

	wbkgd(win, COLOR_PAIR(0));


	move(y-1, x);

	refresh();

	for(int i = 0; i < wid; i++)
		printw(" ");


	wclear(win);

	wrefresh(win);


	delwin(win);


	move(0, 0);

	refresh();
}


void initNCURSES(){

	initscr();
	cbreak();
	noecho();
	keypad(stdscr,  TRUE);
	nodelay(stdscr, TRUE);


	start_color();

	init_pair(0, COLOR_BLACK, COLOR_BLACK);
	init_pair(1, COLOR_WHITE, COLOR_BLUE);
	init_pair(2, COLOR_BLACK, COLOR_CYAN);
	init_pair(3, COLOR_BLUE, COLOR_WHITE);
}


void exitNCURSES(){

	endwin();
}
