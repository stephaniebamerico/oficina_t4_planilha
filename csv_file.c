/*== Stephanie Briere Americo ==*/

#include "table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMVALOR_MAX 255

FILE* retornaArquivo(char *endereco, char *argumento);
void proxCelula(TABELA * tab, unsigned short * lin, unsigned short * col);
void qtdColuna(TABELA * tab, unsigned short * lin, unsigned short * col);

/** O objectivo desta funcao 'e encontrar e
	fragmentar valores que vao em cada celula **/
void leArquivoCSV(TABELA * tab, char* endereco){
	FILE * entr = retornaArquivo(endereco, "r");


	// se estamos lidando com valores entre ""
	char aspas = 0;
	// lin e col atual da tabela
	unsigned short lin = 0, col = 0;
	// c - valor lido, valor - valor da celula
	unsigned char c, valor[TAMVALOR_MAX], tamv = 0;
	// a tabela começa com 0 colunas
	tab->tcol = 0;

	while((fscanf(entr, "%c", &c) > 0) && (lin < LIN_MAX)){
		switch(c){
			case '"':
				// começa/termina um valor entre ""
				aspas = !aspas;
				break;
			case '\n':
				// termina a linha
				// nao pode começar uma linha com "" da anterior
				aspas = 0;

				// se houver algum valor, insere na tabela
				if(tamv > 0){
					valor[tamv] = '\0';
					insereTabela(tab,valor,lin,col);
					tamv = 0;
				}
				
				qtdColuna(tab, &lin, &col);
				++lin;
				col = 0;
				break;
			case ',':
				if(!aspas){
					// terminou a coluna
					// se houver algum valor, insere na tabela
					if(tamv > 0){
						valor[tamv] = '\0';
						insereTabela(tab,valor,lin,col);
						tamv = 0;
					}

					proxCelula(tab, &lin, &col);

					break;
				}
				// se a virgula nao esta entre "", entao trata insere no valor
			default:
				// se for espaço fora de "", ignoramos
				//if(c != ' ' || (c == ' ' && aspas)){
					// inserimos o valor na string
					valor[tamv] = c;
					++tamv;

					if(tamv+1 == TAMVALOR_MAX){
						// nao cabe mais nessa celula
						valor[tamv] = '\0';
						insereTabela(tab,valor,lin,col);
						tamv = 0;

						proxCelula(tab, &lin, &col);
					}
				//}
				break;
		}
	}

	tab->tlin = lin;

	fclose(entr);
}

//* ===================================== *//

void proxCelula(TABELA * tab, unsigned short * lin, unsigned short * col){
	// se ainda nao chegou ao fim da linha
	if(*col+1 < COL_MAX)
		++*col;
	else{
		*lin += 1;
		*col = 0;
	}

	qtdColuna(tab, lin, col);
}

// atualiza a maior qtd de colunas da tabela
void qtdColuna(TABELA * tab, unsigned short * lin, unsigned short * col){
	if(!*lin || tab->tcol < *col) 
		tab->tcol = *col+1;
}

// Abre o arquivo CSV e o retorna 
FILE* retornaArquivo(char *endereco, char *argumento){
	FILE * arq;
	arq = fopen(endereco, argumento);
	if (!arq){
		perror("Erro ao abrir arquivo");
		exit(1);
	}
	
	return arq;
}