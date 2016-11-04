/*== Stephanie Briere Americo ==*/
#include "table.h"
#include "csv_file.h"
#include <stdio.h>
#include <string.h>
#include <ncurses.h>

int main(int argc, char const *argv[]){
	TABELA tabela;
	//leAquivoCSV(&tabela, "example.csv");
	leAquivoCSV(&tabela, "gastos_pessoais.csv");
	//leAquivoCSV(&tabela, "substancias.csv");
	//leAquivoCSV(&tabela, "filtros_de_agua.csv");

	initscr();			
	curs_set(0);

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