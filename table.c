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

void liberaTabela(TABELA * t){
	if (t->celula == NULL){
		return;
	}

	for (int i = 0; i < LIN_MAX+1; ++i){
		if(t->celula[i] != NULL){
			for (int j = 0; j < COL_MAX+1; ++j){			
				if(t->celula[i][j].valor != NULL)
					free(t->celula[i][j].valor);
			}
			free(t->celula[i]);
		}
	}
	free(t->celula);
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
	if(valor[0] == '='){
		// verifica se tem chances de ser uma formula
		// (ainda pode ser invalida)
		int i;
		for (i = 1; valor[i] != '\0' && valor[i] != '+' && valor[i] != '-' 
					&& valor[i] != '*' && valor[i] != '/'; ++i);
		if(valor[i] == '\0')
			tipo = FRASE;
		else{
			int j;
			for (j = i; valor[j] != '\0'; ++j);
			if(j-i > 2 || (valor[i+1] >= '1' && valor[i+1] <= '9'))
				tipo = FORMULA;
			else
				tipo = FRASE;
		}
	}
	else if((valor[0] >= '0' && valor[0] <= '9') || valor[0] == '-')
		tipo = INTEIRO;

	// enquanto for inteiro ou decimal
	for (int i = 1; valor[i] != '\0' && tipo <= INTEIRO; ++i){
		if(valor[i] == '.'){
			if(tipo == INTEIRO)
				tipo = DECIMAL;
		}
		// se nao for um numero
		else if(valor[i] < '0' && valor[i] > '9')
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

void calculaFormula(TABELA * tab, unsigned short lin, unsigned short col, char temp[300]){
	int i, achou=0, tipo;
	// define a operacao e sua posicao no vetor
	for(i=0; tab->celula[lin][col].valor[i] != '\0' && !achou; ++i){
		switch(tab->celula[lin][col].valor[i]){
			case '+':
				achou = 1;
				break;
			case '-':
				achou = 2;
				break;
			case '*':
				achou = 3;
				break;
			case '/':
				achou = 4;
				break;
		}
	}
	--i;
	strcpy(temp, tab->celula[lin][col].valor);
	// verifica se sao numeros ou referencias a celulas
	tipo = INTEIRO;
	int n1tipo=INTEIRO, n2tipo=INTEIRO;
	for (int j = 1; temp[j] != '\0'; ++j){
		if(temp[j] == '.')
			tipo = DECIMAL;
		else if(temp[j] >= 'A' && temp[j] <= 'Z'){
			if(j < i)
				n1tipo = FRASE;
			else if (j > i)
				n2tipo = FRASE;
		}
	}
	// calcula o valor, seja numero ou celula
	double n1, n2, r;
	if(n1tipo == FRASE){
		int c = temp[1]-'A';
		int l=0;// = temp[2]-'1';
		
		l = atoi(temp+2)-1;
		if(l > LIN_MAX || c > COL_MAX || l < 0 || c < 0
			|| tab->celula[l][c].valor == NULL)
			n1 = 0;
		else{
			if(tab->celula[l][c].tipo == FORMULA){
				char t[300];
				calculaFormula(tab, l, c, t);
				n1 = atof(t);
			}
			else if(tab->celula[l][c].tipo == INTEIRO || tab->celula[l][c].tipo == DECIMAL)
				n1 = atof(tab->celula[l][c].valor);
			else
				n1=0;
		}
	}
	else{
		n1 = atof(temp+1);
	}
	temp[i]='\0';
	if(n2tipo == FRASE){
		int c = temp[i+1]-'A';
		int l=0;// = temp[i+2]-'1';

		l = atoi(temp+i+2)-1;
		if(l > LIN_MAX || c > COL_MAX || l < 0 || c < 0
			|| tab->celula[l][c].valor == NULL)
			n2 = 0;
		else{
			if(tab->celula[l][c].tipo == FORMULA){
				char t[300];
				calculaFormula(tab, l, c, t);
				n2 = atof(t);
			}
			else if(tab->celula[l][c].tipo == INTEIRO || tab->celula[l][c].tipo == DECIMAL)
				n2 = atof(tab->celula[l][c].valor);
			else
				n2=0;
		}
	}
	else{
		n2 = atof(temp+i+1);
	}

	// realiza a operacao
	switch(achou){
		case 1:
			r = n1+n2;
			break;
		case 2:
			r = n1-n2;
			break;
		case 3:
			r = n1*n2;;
			break;
		case 4:
			r = n1/n2;
			break;
	}
	// verifica se o resultado e inteiro ou decimal
	// (caso o calculo foi com celula)
	if(tipo == INTEIRO){
		long n = (long) r;
		if(r - n > 0)
			tipo = DECIMAL;
	}
	// formata a saida em string
	if(tipo == INTEIRO){
		sprintf(temp, "%ld", (long) r);
	}
	else{
		sprintf(temp, "%lf", r);
	}
}