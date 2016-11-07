/*== Stephanie Briere Americo ==*/

#ifndef __TAB__
#define __TAB__

#include <stdio.h>

#define LIN_MAX 100
#define COL_MAX 26
#define TAMVALOR_MAX 255

#define FRASE 0
#define FORMULA 1
#define INTEIRO 2
#define DECIMAL 3


typedef struct CELULA {
	unsigned short tipo; // 0 - "str", 1 - "fml", 2 - "nui" ou 3 - "nud"
	char *valor; 

} CELULA;

typedef struct TABELA {
	unsigned short int tlin, tcol;
	CELULA **celula;//[LIN_MAX+1][COL_MAX+1]; 
} TABELA;

void alocaTabela(TABELA * t);
void insereTabela(TABELA * tab, unsigned char *valor, unsigned short lin, unsigned short col);
void limpaValores(TABELA * tab);
void calculaFormula(TABELA * tab, unsigned short lin, unsigned short col, char temp[300]);
void liberaTabela(TABELA * t);
#endif