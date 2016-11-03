#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

int main()
{	
	srand(time(NULL));
	char hello[] = "Hello World !!!";
	initscr();			
	curs_set(0);

	int i = 0;
	int x,y,maxx,maxy, color;
	
	start_color();

	init_pair(1,COLOR_RED,COLOR_BLACK);
	init_pair(2,COLOR_GREEN,COLOR_BLACK);
	init_pair(3,COLOR_YELLOW,COLOR_BLACK);
	init_pair(4,COLOR_BLUE,COLOR_BLACK);
	init_pair(5,COLOR_MAGENTA,COLOR_BLACK);
	init_pair(6,COLOR_CYAN,COLOR_BLACK);


	

	while(1)
	{
		getmaxyx(stdscr,maxy,maxx);
		x = rand() %(maxx+1);
		y = rand() %(maxy+1);

		color = rand() % 6 + 1;
		attron(COLOR_PAIR(color) | A_BOLD);
		mvprintw(y,x,"%c", hello[i]);
		attroff(COLOR_PAIR(color) | A_BOLD);

		attron(COLOR_PAIR(2) | A_BOLD);
		mvprintw(maxy/2-1,maxx/2-strlen(hello)/2-3,"                     ");
		mvprintw(maxy/2,maxx/2-strlen(hello)/2-3,"   %s   ",hello);	
		mvprintw(maxy/2+1,maxx/2-strlen(hello)/2-3,"                     ");
		attroff(COLOR_PAIR(2) | A_BOLD);

		refresh();
		usleep(10000);
	
	i = (i+1)%strlen(hello);
				
	}			
	endwin();			

	return 0;
}

