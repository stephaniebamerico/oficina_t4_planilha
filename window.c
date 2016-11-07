#include "window.h"

void imprimeCursor(JANELA j, TABELA * tabela, int e);
void imprimeAncora(JANELA j, TABELA * tabela);

void iniciaJanela(JANELA * j){
	initscr(); // inicia			
	curs_set(0); // cursor
	timeout(0); // esparar por entrada
	noecho(); // sem echo
	raw();  // leitura por char
	keypad(stdscr,true); // leitura do teclado
	start_color();

	j->cursor.lin = j->cursor.col = 0;
	j->ini.lin = j->ini.col = 0;
	j->fin.lin = j->fin.col = 0;
	j->ancora.lin = j->ancora.col = 0;
	calculaCelulasJanela(j);



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
}

void encerraJanela(){
	curs_set(1); // cursor
	timeout(-1); // esparar por entrada
	echo(); // sem echo
	cbreak();  // leitura por char
	endwin(); // encerra			
}

void calculaCelulasJanela(JANELA * j){
	int maxx, maxy, lin, col, dif;
	getmaxyx(stdscr,maxy,maxx);

	// calcula a quantidade de col/lin da tabela que vao aparecer na janela
	col = (maxx/LARG_CELULA < COL_MAX) ? maxx/LARG_CELULA : COL_MAX;
	lin = (maxy < LIN_MAX) ? maxy -1 : LIN_MAX -1;

	// se alterou o tamanho da janela
	dif = (lin - (j->fin.lin - j->ini.lin)) + (col - (j->fin.col - j->ini.col));
	if(dif){
		//calcula ate onde vai mostrar a tabela agora
		j->fin.lin = (lin + j->ini.lin < LIN_MAX) ? lin + j->ini.lin : LIN_MAX;
		j->fin.col = (col + j->ini.col < COL_MAX) ? col + j->ini.col : COL_MAX;
		
		//se o cursor foi escondido, coloca em 0,0
		if(j->cursor.lin >= j->fin.lin || j->cursor.col >= j->fin.col){
			j->cursor.lin = j->ini.lin;
			j->cursor.col = j->ini.col;
		}
	}
}

void desenhaBarras(JANELA j){
	attron(COLOR_PAIR(j.cor + N_CORES) | A_BOLD);
		// barra col
		for (int i = 0; i < j.fin.col - j.ini.col; ++i){
			for (int k = 0; k < LARG_CELULA; ++k)
				mvprintw(0, 3 + i*LARG_CELULA + k,"%c", ' ');
			mvprintw(0, i*LARG_CELULA + 2*LARG_CELULA/3,
				"%c", 'A'+j.ini.col+i);
		}
		// barra lin
		for (int i = 1; i <= j.fin.lin - j.ini.lin; ++i){
			for (int j = 0; j < 3; ++j)
				mvprintw(i, j,"%c", ' ');
			mvprintw(i, 0,"%d", i+j.ini.lin);
		}
	attroff(COLOR_PAIR(j.cor + N_CORES) | A_BOLD);
}

void imprimeTabela(JANELA j, TABELA * tabela, int e, int a){
	// imprime as celulas que aparecem na janela
	attron(COLOR_PAIR(j.cor) | A_BOLD);
		for (int i = 0; i < j.fin.lin - j.ini.lin; ++i){
			for (int k = 0; k < j.fin.col - j.ini.col; ++k){
				if(tabela->celula[j.ini.lin+i][j.ini.col+k].valor != NULL){
					if(tabela->celula[j.ini.lin+i][j.ini.col+k].tipo != FORMULA){
						mvprintw(i+1, k*LARG_CELULA + LARG_CELULA/2,
						"%.5s", tabela->celula[j.ini.lin+i][j.ini.col+k].valor);
					}else{
						char t[300];
						calculaFormula(tabela, j.ini.lin+i, j.ini.col+k, t);
						mvprintw(i+1, k*LARG_CELULA + LARG_CELULA/2,
						"%.5s", t);
					}
				}
			}
		}
	attroff(COLOR_PAIR(j.cor) | A_BOLD);

	if(a)
		imprimeAncora(j,  tabela);
	imprimeCursor(j, tabela, e);
}

void imprimeCursor(JANELA j, TABELA * tabela, int e){
	//imprime cursor na cor inversa
	int linha = j.cursor.lin-j.ini.lin+1;
	int coluna = 3 + (j.cursor.col-j.ini.col)*LARG_CELULA;
	int cor = (e) ? COR_Black_Magenta : j.cor + N_CORES;
	attron(COLOR_PAIR(cor) | A_BOLD);
		for (int k = 0; k < LARG_CELULA; ++k)
			mvprintw(linha, coluna + k,"%c", ' ');
		if(tabela->celula[j.cursor.lin][j.cursor.col].valor != NULL){
			mvprintw(linha, coluna + 1,
			"%.5s", tabela->celula[j.cursor.lin][j.cursor.col].valor);
		}
	attroff(COLOR_PAIR(cor) | A_BOLD);
}

void imprimeAncora(JANELA j, TABELA * tabela){
	int li, lf, ci, cf;
	calculaAreaSelecionada(j, &li, &lf, &ci, &cf);

	attron(COLOR_PAIR(COR_Black_Blue) | A_BOLD);
		for (; li <= lf; ++li){
			for (int i = ci; i <= cf; ++i){
				for (int k = 0; k < LARG_CELULA; ++k)
					mvprintw(li+1, 3 + i*LARG_CELULA + k," ");
				if(tabela->celula[li][i].valor != NULL){
					mvprintw(li+1, i*LARG_CELULA + LARG_CELULA/2,
					"%.5s", tabela->celula[li][i].valor);
				}
			}
		}
	attroff(COLOR_PAIR(COR_Black_Blue) | A_BOLD);
}

void atualizaTela(JANELA * j){
	refresh();
	usleep(SLEEP);
	clear();
	//j->cor = j->cor%N_CORES +1;
}

void moveCursor(JANELA * j, int lin, int col){
	if (lin && j->cursor.lin + lin >= 0 && j->cursor.lin + lin < LIN_MAX){
		j->cursor.lin += lin;

		// se precisa ajustar o que mostrar na tabela
		if(j->ini.lin > j->cursor.lin){
			j->ini.lin--;
			j->fin.lin--;
		} else if(j->fin.lin <= j->cursor.lin){
			j->ini.lin++;
			j->fin.lin++;
		}
	}
	else if (col && j->cursor.col + col >= 0 && j->cursor.col + col < COL_MAX){
		j->cursor.col += col;

		// se precisa ajustar o que mostrar na tabela
		if(j->ini.col > j->cursor.col){
			j->ini.col--;
			j->fin.col--;
		} else if(j->fin.col <= j->cursor.col){
			j->ini.col++;
			j->fin.col++;
		}
	}
}

void imprimeFrase(char * f, int lin, int col, int cor){
	attron(COLOR_PAIR(cor) | A_BOLD);
		mvprintw(lin, col,f);
	attroff(COLOR_PAIR(cor) | A_BOLD);
}

void calculaAreaSelecionada(JANELA j, int * li, int * lf, int * ci, int * cf){
	if(j.cursor.lin < j.ancora.lin){
		*li = j.cursor.lin;
		*lf = j.ancora.lin;
	}
	else{
		*li = j.ancora.lin;	
		*lf = j.cursor.lin;
	}

	if(j.cursor.col < j.ancora.col){
		*ci = j.cursor.col;
		*cf = j.ancora.col;
	}
	else{
		*ci = j.ancora.col;	
		*cf = j.cursor.col;
	}
}