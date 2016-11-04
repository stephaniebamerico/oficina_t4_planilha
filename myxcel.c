/*== Stephanie Briere Americo ==*/
#include "table.h"
#include "csv_file.h"
#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>

int main(int argc, char const *argv[]){
	TABELA tabela;
	leArquivoCSV(&tabela, "example.csv");
	//leAquivoCSV(&tabela, "gastos_pessoais.csv");
	//leAquivoCSV(&tabela, "substancias.csv");
	//leAquivoCSV(&tabela, "filtros_de_agua.csv");
	initscr();			
	curs_set(0);

	start_color();

	init_pair(1,COLOR_RED,COLOR_BLACK);
	init_pair(2,COLOR_GREEN,COLOR_BLACK);
	init_pair(3,COLOR_YELLOW,COLOR_BLACK);
	init_pair(4,COLOR_BLUE,COLOR_BLACK);
	init_pair(5,COLOR_MAGENTA,COLOR_BLACK);
	init_pair(6,COLOR_CYAN,COLOR_BLACK);

	int maxx, maxy, color = 1;
	while(1){
		getmaxyx(stdscr,maxy,maxx);

		attron(COLOR_PAIR(color) | A_BOLD);
		mvprintw(maxy/2,maxx/10 + 8*color,"%s", "HELLO");
		/*for (int i = 0; i < tabela.tlin; ++i){
			for (int j = 0; j < tabela.tcol; ++j){
				mvprintw(maxy*i, maxx*j,"'%.5s'", tabela.celula[i][j].valor);
			}
		}*/


		attroff(COLOR_PAIR(color) | A_BOLD);

		refresh();
		usleep(1000000);
		clear();
		color = color%6 +1;			
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