/*== Stephanie Briere Americo ==*/

#ifndef __WINDOW__
#define __WINDOW__

#include <ncurses.h>
#include <unistd.h>
#include "table.h"

#define LARG_CELULA 8
#define N_CORES 6
#define SLEEP 200000
#define ENTER 10
#define BACKSPACE 263
#define CTRL_SPACE 0
#define CTRL_C 3
#define CTRL_Q 17
#define CTRL_R 18
#define CTRL_S 19

#define COR_Red_Black 1
#define COR_Green_Black 2
#define COR_Yellow_Black 3
#define COR_Blue_Black 4
#define COR_Magenta_Black 5
#define COR_Cyan_Black 6
#define COR_Black_Red 7
#define COR_Black_Green 8
#define COR_Black_Yellow 9
#define COR_Black_Blue 10
#define COR_Black_Magenta 11
#define COR_Black_Cyan 12

typedef struct COORDENADAS {
	unsigned short int lin, col; 
} COORDENADAS;

typedef struct JANELA {
	COORDENADAS cursor;
	COORDENADAS ancora;
	COORDENADAS ini, fin; // tabela mostrada na janela
	int cor; 
} JANELA;

void iniciaJanela(JANELA * j);
void encerraJanela();
void calculaCelulasJanela(JANELA * j);
void desenhaBarras(JANELA j);
void imprimeTabela(JANELA j, TABELA * tabela, int e, int a);
void atualizaTela(JANELA * j);
void moveCursor(JANELA * j, int lin, int col);
void imprimeFrase(char * f, int lin, int col, int cor);

#endif