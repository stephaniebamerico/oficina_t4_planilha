/*== Stephanie Briere Americo ==*/

#include "table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void alocaTabela(TABELA * t){
	t->celula = (CELULA **) malloc ((LIN_MAX+1)*sizeof(CELULA *));

	if (t->celula == NULL){
		perror("Erro ao alocar memoria");
		exit(1);
	}

	for (int i = 0; i < LIN_MAX+1; ++i){
		t->celula[i] = (CELULA *) malloc ((COL_MAX+1)*sizeof(CELULA));
	}

	t->tlin = t->tcol = 0;
}

/** O objectivo desta funcao 'e receber os valores da celula,
	determinar o tipo do valor e inseri-los na tabela **/
void insereTabela(TABELA * tab, unsigned char *valor, unsigned short lin, unsigned short col){
	// se a celula ja estava alocada, desaloca
	if(tab->celula[lin][col].valor != NULL){
		free(tab->celula[lin][col].valor);
	}

	// aloca nova celula
	tab->celula[lin][col].valor = (char *) malloc(300);

	if (tab->celula[lin][col].valor == NULL){
		perror("Erro ao alocar memoria");
		exit(1);
	}
	
	// define o tipo de valor
	unsigned short tipo = FRASE;
	if(valor[0] == '=')
		tipo = FORMULA;
	else if((valor[0] > '0' && valor[0] < '9') || valor[0] == '-')
		tipo = INTEIRO;

	// enquanto for inteiro ou decimal
	for (int i = 1; valor[i] != '\0' && tipo != FRASE; ++i){
		if(valor[i] == '.'){
			if(tipo == INTEIRO)
				tipo = DECIMAL;
			// se for o segundo . ja nao pode ser decimal
			else
				tipo = FRASE;
		}
		// se nao for um numero
		else if((valor[i] < '0' && valor[i] > '9') && tipo != FORMULA)
			tipo = FRASE;
	}
	// guarda o tipo e o valor na celula
	tab->celula[lin][col].tipo = tipo;
	strcpy(tab->celula[lin][col].valor, (char *) valor);

	if(tab->tlin < lin)
		tab->tlin = LIN_MAX;
	if(tab->tcol < col)
		tab->tcol = col;
}

void limpaValores(TABELA * tab){
	for(int i = 0; i <= tab->tlin; ++i){
		for(int j = 0; j <= tab->tcol; ++j){
			if(tab->celula[i][j].valor != NULL){
				free(tab->celula[i][j].valor);
				tab->celula[i][j].valor = NULL;
			}
		}
	}

	tab->tlin = tab->tcol = 0;
}