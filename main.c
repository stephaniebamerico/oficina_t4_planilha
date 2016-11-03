/*== Stephanie Briere Americo ==*/
#include "tabela.h"
#include "manipula_tabela.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[]){
	TABELA tabela;
	//leAquivoCSV(&tabela, "example.csv");
	leAquivoCSV(&tabela, "gastos_pessoais.csv");
	//leAquivoCSV(&tabela, "substancias.csv");
	//leAquivoCSV(&tabela, "filtros_de_agua.csv");
	int n;
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
	}

	return 0;
}