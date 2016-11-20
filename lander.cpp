#include "headers/getch.h"
#include <ncurses.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <fcntl.h>
#include <time.h>


int kbhit(void){
	struct termios oldt, newt;
	int ch;
	int oldf;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);
	if(ch != EOF){
		// ungetc(ch, stdin);
		return 1;
	}
	return 0;
}

void sleep_ms(int milliseconds){
#ifdef WIN32
	Sleep(milliseconds);
#elif _POSIX_C_SOURCE >= 199309L
	struct timespec ts;
	ts.tv_sec = milliseconds / 1000;
	ts.tv_nsec = (milliseconds % 1000) * 1000000;
	nanosleep(&ts, NULL);
#else
	usleep(milliseconds * 1000);
#endif
}

int main(){

	initscr();
	curs_set(0);
	int startPoint = 15;
	int rockstart = 0;
	int loops = 0;
	int consoleGraph = 1;
	int randomXn = 15;
	int newRock = 0;
	int randomX[1];

	int wtf = 0;


	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	printw("lines %d", w.ws_row);
	printw("columns %d", w.ws_col);


	while(true){
		char key;
		struct winsize w;

		ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);	// GET THE TERMINAL SIZE
		clear();

		
		while( ! kbhit() ){

			clear();
			
			if ( rockstart > w.ws_row ){
				++wtf;
				rockstart = 0;
				newRock = 1;
			}

			if (kbhit()){
				key = localGetch();
				break;
			}

			if ( newRock == 1 );{
				randomXn = 3 + (rand() % (int)((w.ws_col - 6 ) - 3 + 1));
				randomX[1] = randomXn;
				newRock = 0;
			}
			mvprintw(rockstart,randomX[1],"X");
			++rockstart;				// END ROCK

			if (kbhit()){
				key = localGetch();
				break;
			}

			sleep_ms(50);

			if (kbhit()){
				key = localGetch();
				break;
			}

			++loops;

			for (int i = 0; i < w.ws_row; ++i){		// BORDERS
				mvprintw(i,3,"|");
				mvprintw(i,w.ws_col - 3,"|");
			}

			if (kbhit()){
				key = localGetch();
				break;
			}

			mvprintw(w.ws_row - 3,startPoint,"/A\\");

			if ( consoleGraph == 1 ){
				mvprintw(0,4,"lines %d\n", w.ws_row);
				mvprintw(1,4,"columns %d\n", w.ws_col);
				mvprintw(2,4,"Cursor at x:%i", startPoint);
				mvprintw(3,4,"Rock y:%i x:%i", rockstart, randomX[1]);
				mvprintw(4,4,"Loops %i", loops);
				if (kbhit()){
					mvprintw(5,4,"kbhit is at 1");
				} else{
					mvprintw(5,4,"kbhit is at 0");
				}
				mvprintw(6,4,"newRock val:%i",newRock);
				mvprintw(7,4,"wtf:%i", wtf);
			}
			refresh();
		}
		
		clear();

		mvprintw(rockstart,randomX[1],"X");

		for (int i = 0; i < w.ws_row; ++i){		// BORDERS
			mvprintw(i,3,"|");
			mvprintw(i,w.ws_col - 3,"|");
		}

		
		if ( key == 'z' ){
			if ( startPoint == 4 ){
				continue;
			} else{
				startPoint = startPoint - 1;
				mvprintw(w.ws_row - 3,startPoint,"/A\\");
			}
		} else if ( key == 'x' ){
			if ( startPoint == w.ws_col - 4 ){
				continue;			
			} else{
				startPoint = startPoint + 1;
				mvprintw(w.ws_row - 3,startPoint,"/A\\");
			}
		} else {
			continue;
		}
		
		if ( consoleGraph == 1 ){
			mvprintw(0,4,"lines %d\n", w.ws_row);
			mvprintw(1,4,"columns %d\n", w.ws_col);
			mvprintw(2,4,"Cursor at x:%i", startPoint);
			mvprintw(3,4,"Rock y:%i x:%i", rockstart, randomX[1]);
			mvprintw(4,4,"Loops %i", loops);
			if (kbhit()){
				mvprintw(5,4,"kbhit is at 1");
			} else{
				mvprintw(5,4,"kbhit is at 0");
			}
			mvprintw(6,4,"newRock val:%i",newRock);
			mvprintw(7,4,"wtf:%i", wtf);
		}

		refresh();
	}
	endwin();
	return 0;
}