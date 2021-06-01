/*
 * termip.c
 *
 *  Created on: 12 de mai de 2017
 *      Author: Gustavo Teodoro Laureano
 *		email: gustavo@inf.ufg.br
 */
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <termios.h>    //termios, TCSANOW, ECHO, ICANON

#include "termip_defs.h"

void eval( const char * code ) {
	printf( "%s", code );
}

/**
 * Same of getchar without '\n' and with timeout delay.
 * It gets the key pressed. This function waits a key
 * in time interval sec (in seconds) + usec (in microseconds).
 * If neither key is pressed it returns -1.
 * @param sec seconds
 * @param usec microseconds
 * @return code of key pressed
 */
int getch_timeout( int sec, int usec ) {
	int select_result;
	fd_set read_fds;
	struct timeval timeout;
	int c = -1, t = 0;
	static struct termios oldt, newt;

	tcgetattr( STDIN_FILENO, &oldt); // get stdin conf
	newt = oldt;	// save stdin conf
	newt.c_lflag &= ~(ICANON | ECHO); // update stdin conf
	tcsetattr( STDIN_FILENO, TCSANOW, &newt); // set stdin conf
	   
	FD_ZERO( &read_fds );
	FD_SET( STDIN_FILENO, &read_fds );
   
	timeout.tv_sec = sec;
	timeout.tv_usec = usec;
	select_result = select( 1, &read_fds, NULL, NULL, &timeout );
	
	if (select_result) {
		c = getchar();
		if (c == '\033') {
			c = getchar();
			if (c == '[') {
				t = c;
				do {
					c = getchar();
					t = t << 8 | c;
				} while (c < '@' || c > '~');
				c = t;
			}
		}
	}
	tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
	return c;
}

/**
 * Same of getchar without '\n'. Get the key immediately pressed.
 * 
 * @return ASCII code of pressed key.
 */
int getch( void ) {
	int c, t = 0;
	static struct termios oldt, newt;
	tcgetattr( STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr( STDIN_FILENO, TCSANOW, &newt);
	c = getchar();
	if (c == '\033') {
		c = getchar();
		if (c == '[') {
			t = c;
			do {
				c = getchar();
				t = t << 8 | c;
			} while (c < '@' || c > '~');
			c = t;
		}
	}

	tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
	return c;

}

/**
 * Clear screen
 */
void clear() {
	eval(CLEAR);
}

//void clrscr() {
//	int r, c, i;
//	r = get_terminal_nrows();
//	c = get_terminal_ncols();
//	for (i = 0; i < r * c; i++)
//		printf(" ");
//}

void gotoxy(int row, int col) {
	printf(CURSOR(%d, %d), row, col);
}

void set_fg(int color, char b) {
	if (b) printf("\033[%dm", color + 90);
	else printf("\033[%dm", color + 30);
}

void set_bg(int color, char b) {
	if (b) printf("\033[%dm", color + 100);
	else printf("\033[%dm", color + 40);
}

void set_fg_color_mode(int color) {
	printf(FG_COLOR_MODE(%d), color);
}

void set_bg_color_mode(int color) {
	printf(BG_COLOR_MODE(%d), color);
}

void set_fg_rgb(int r, int g, int b) {
	printf( FG_RGB(%d, %d, %d), r, g, b);
}
void set_bg_rgb(int r, int g, int b) {
	printf(BG_RGB(%d, %d, %d), r, g, b);
}

int get_terminal_ncols() {
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	return w.ws_col;
}
int get_terminal_nrows() {
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	return w.ws_row;
}

void cursor_up(int n) {
	printf(CURSOR_UP(%d), n); // Move up X lines;
}
void cursor_down(int n) {
	printf(CURSOR_DOWN(%d), n); // Move down X lines;
}

void cursor_righ(int n) {
	printf(CURSOR_FORWARD(%d), n); // Move right X column;
}

void cursor_left(int n) {
	printf(CURSOR_BACK(%d), n); // Move left X column;
}

void scrow_up(int n) {
	printf(SCROLL_UP(%d), n);
}
void scrow_down(int n) {
	printf(SCROLL_DOWN(%d), n);
}



void draw_window_border(int r, int c, int width, int height, const char * name) {
	int i;

	for (i = 1; i < width; i++) {
		gotoxy(r, c + i);
		printf("─"); // printf("\u2500");
		gotoxy(r + height, c + i);
		printf("─");
	}

	gotoxy(r, c + 2);
	printf("%s", name);

	for (i = 1; i < height; i++) {
		gotoxy(r + i, c);
		printf("│"); // printf("\u2502");
		gotoxy(r + i, c + width);
		printf("│");
	}

	// top left
	gotoxy(r, c);
	printf("┌"); // printf("\u250c");

	// top right
	gotoxy(r, c + width);
	printf("┐"); // printf("\u2510");

	// down left
	gotoxy(r + height, c);
	printf("└"); // printf("\u2514");

	// down right
	gotoxy(r + height, c + width);
	printf("┘"); //printf("\u2518");
}


void draw_window(int row, int col, int width, int height, const char * border, const char * back, const char * name) {
	int r, c;

	eval( ATTR_RESET_ALL );
	eval( back ) ;
	for( r = 1; r < height; r++ ) {
		gotoxy(r+row,col);
		for( c = 1; c <= width; c++ ) printf(" ");
	}

	eval( ATTR_RESET_ALL );
	eval( border );
	draw_window_border(row, col, width, height, name);

	eval( ATTR_RESET_ALL );
}
