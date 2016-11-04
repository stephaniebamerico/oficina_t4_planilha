/*== Stephanie Briere Americo ==*/
#include "table.h"
#include "csv_file.h"
#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>

#define LARG_CELULA 8
#define N_CORES 6

int main(int argc, char const *argv[]){
	TABELA tabela;
	//leArquivoCSV(&tabela, "example.csv");
	//leArquivoCSV(&tabela, "gastos_pessoais.csv");
	//leArquivoCSV(&tabela, "substancias.csv");
	leArquivoCSV(&tabela, "filtros_de_agua.csv");
	initscr();			
	curs_set(0);

	start_color();

	init_pair(1,COLOR_RED,COLOR_BLACK);
	init_pair(2,COLOR_GREEN,COLOR_BLACK);
	init_pair(3,COLOR_YELLOW,COLOR_BLACK);
	init_pair(4,COLOR_BLUE,COLOR_BLACK);
	init_pair(5,COLOR_MAGENTA,COLOR_BLACK);
	init_pair(6,COLOR_CYAN,COLOR_BLACK);
	init_pair(7,COLOR_BLACK,COLOR_RED);
	init_pair(8,COLOR_BLACK,COLOR_GREEN);
	init_pair(9,COLOR_BLACK,COLOR_YELLOW);
	init_pair(10,COLOR_BLACK,COLOR_BLUE);
	init_pair(11,COLOR_BLACK,COLOR_MAGENTA);
	init_pair(12,COLOR_BLACK,COLOR_CYAN);

	//int maxx, maxy;
	int color = 1;
	while(1){
		//getmaxyx(stdscr,maxy,maxx);

		attron(COLOR_PAIR(color + N_CORES) | A_BOLD);
		for (int i = 1; i < COL_MAX; ++i){
			for (int j = 0; j < LARG_CELULA; ++j)
				mvprintw(0, 2+(i-1)*LARG_CELULA + j,"%c", ' ');
			mvprintw(0, i*LARG_CELULA + 2*LARG_CELULA/3,"%c", 'A'+i);
		}
		for (int i = 1; i < LIN_MAX; ++i){
			for (int j = 0; j < LARG_CELULA/4; ++j)
				mvprintw(i, j,"%c", ' ');
			mvprintw(i, 0,"%d", i);
		}
		attroff(COLOR_PAIR(color + N_CORES) | A_BOLD);

		attron(COLOR_PAIR(color) | A_BOLD);
		for (int i = 1; i < tabela.tlin; ++i){
			for (int j = 1; j < tabela.tcol; ++j){
				mvprintw(i, j*LARG_CELULA + 1,"'%.5s'", tabela.celula[i][j].valor);
			}
		}
		attroff(COLOR_PAIR(color) | A_BOLD);

		refresh();
		usleep(1000000);
		clear();
		color = color%N_CORES +1;			
	}			
	endwin();

	/*int n;
	printf("%d %d\n", tabela.tlin, tabela.tcol);
	for (int i = 0; i < tabela.tlin; ++i){
		for (int j = 0; j < tabela.tcol; ++j){
			n = (tabela.celula[i][j].valor == NULL) ? 5 : 5 - strlen(tabela.celula[i][j].valor);
			printf("'%.5s", tabela.celula[i][j].valor);
			for (int k = 0; k < n; ++k)
				printf(" ");
			printf("' ");
		}
		printf("\n");
	}

	for (int i = 0; i < tabela.tlin; ++i){
		for (int j = 0; j < tabela.tcol; ++j){
			switch(tabela.celula[i][j].tipo){
				case FRASE:
					printf("STR ");
					break;
				case FORMULA:
					printf("FML ");
					break;
				case INTEIRO:
					printf("NUI ");
					break;
				case DECIMAL:
					printf("NUD ");
					break;
			}
		}
		printf("\n");
	}*/

	return 0;
}