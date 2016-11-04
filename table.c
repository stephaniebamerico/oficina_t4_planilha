/*== Stephanie Briere Americo ==*/

#include "table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** O objectivo desta funcao 'e receber os valores da celula,
	determinar o tipo do valor e inseri-los na tabela **/
void insereTabela(TABELA * tab, unsigned char *valor, unsigned short lin, unsigned short col){
	// se a celula ja estava alocada, desaloca
	if(tab->celula[lin][col].valor != NULL){
		free(tab->celula[lin][col].valor);
		//free(tab->celula[lin][col]);
	}

	// aloca nova celula
	tab->celula[lin][col].valor = (char *) malloc(sizeof(char *));
	
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
	//printf("%s\n", tab->celula[lin][col].valor);
}

/*long calculaFormula(TABELA * tab, unsigned short lin, unsigned short col){
	if(tab->celula[lin][col].tipo != FORMULA)
		perror("A expressao indicada nao e uma formula");
	else{
		unsigned int op;
		int n1, n2;
		for(int i = 1; i < strlen(tabela.celula[i][j].valor && ); ++i){
			
			
		}
	}

	return 0;
}*/