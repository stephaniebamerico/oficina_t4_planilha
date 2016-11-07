/*== Stephanie Briere Americo ==*/
#include "table.h"
#include "csv_file.h"
#include "window.h"
#include <stdlib.h>
#include <string.h>

void entr(int * e, int * cont);
void insereNaString(unsigned char * t, int ch, int * cont);
void alocaCopia(CELULA ***c);
void copiaValores(TABELA * tab, JANELA j, CELULA **c, COORDENADAS * area, int x);
void colaValores(TABELA * tab, JANELA j, CELULA **c, COORDENADAS area);

int main(int argc, char const *argv[]){
	TABELA tabela;
	JANELA j; 
	CELULA **copia;
	COORDENADAS area;
	int ch, cont, e=0, r=0, s=0, a=0;
	unsigned char t[255];
	
	alocaTabela(&tabela);
	//alocaCopia(&copia);
	copia = (CELULA **) malloc ((LIN_MAX+1)*sizeof(CELULA *));

	if (copia == NULL){
		perror("Erro ao alocar memoria");
		exit(1);
	}

	for (int i = 0; i < LIN_MAX+1; ++i)
		copia[i] = (CELULA *) malloc ((COL_MAX+1)*sizeof(CELULA));

	char arq[255];
	if(argc > 1){ 
		strcpy(arq, argv[1]);
		printf("%s\n", arq);
		if(!arquivoValido(arq)){
			perror("Erro ao abrir arquivo");
			exit(1);
		}
		else
			leArquivoCSV(&tabela, arq);
	}
	else
		arq[0] = '\0';

	j.cor = COR_Cyan_Black; 
	iniciaJanela(&j);
	while(ch != CTRL_Q){
		calculaCelulasJanela(&j);
		desenhaBarras(j);
		imprimeTabela(j, &tabela, e, a);
		if(r || s){
			imprimeFrase("ENTRE COM ARQUIVO: ", j.fin.lin-j.ini.lin, 3, j.cor + N_CORES);
			imprimeFrase((char *) t, j.fin.lin-j.ini.lin, 22, j.cor + N_CORES);
		}

		atualizaTela(&j);

		ch = wgetch(stdscr);
		switch(ch){
			case KEY_LEFT:
				entr(&e, &cont);
				moveCursor(&j, 0, -1);
				break;
			case KEY_UP:
				entr(&e, &cont);
				moveCursor(&j, -1, 0);
				break;
			case KEY_RIGHT:
				entr(&e, &cont);
				moveCursor(&j, 0, 1);
				break;
			case KEY_DOWN:
				entr(&e, &cont);
				moveCursor(&j, 1, 0);
				break;
			case CTRL_SPACE:
				entr(&e, &cont);
				if(!a){
					j.ancora.lin = j.cursor.lin; 
					j.ancora.col = j.cursor.col;
					a=1;
				}
				else{
					j.ancora.lin = j.ancora.col = 0;
					a=0;
				}		
				break;
			case CTRL_C:
				if(!a){
					j.ancora.lin = j.cursor.lin;
					j.ancora.col = j.cursor.col;
				}
				copiaValores(&tabela, j, copia, &area, 0);
				a=0;
				break;
			case CTRL_V:
				colaValores(&tabela, j, copia, area);
				a=0;
				break;
			case CTRL_X:
				if(!a){
					j.ancora.lin = j.cursor.lin;
					j.ancora.col = j.cursor.col;
				}
				copiaValores(&tabela, j, copia, &area, 1);
				a=0;
				break;
			case CTRL_R:
				entr(&e, &cont);
				if(!r)
					t[0]='\0';
				r=!r;
				cont=0;
				break;
			case CTRL_S:
				entr(&e, &cont);
				if(arq[0] != '\0'){
					escreveAquivoCSV(&tabela, arq);
					imprimeFrase("ARQUIVO SALVO", j.fin.lin-j.ini.lin, 10, j.cor + N_CORES);
				}
				else{
					s=1;
					cont=0;
					t[cont]='\0';
				}
				break;
			case BACKSPACE:
				// remove o ultimo char
				if((e || r || s) && cont > 0){
					cont--;
					t[cont]='\0';
					if(e)
						insereTabela(&tabela, t, j.cursor.lin, j.cursor.col);
				}
				break;
			case ENTER:
				// enter para o CTRL_R
				if(r){
					if(arquivoValido((char *) t)){
						limpaValores(&tabela);
						leArquivoCSV(&tabela, (char *) t);
						strcpy(arq, (char *) t);
					}
					else
						imprimeFrase("ARQUIVO NAO ENCONTRADO", j.fin.lin-j.ini.lin, 10, COR_Black_Red);
					r=0;
				}
				else if(s){
					if(t[0] != '\0'){
						strcpy(arq, (char *) t);
						escreveAquivoCSV(&tabela, arq);
						imprimeFrase("ARQUIVO SALVO", j.fin.lin-j.ini.lin, 10, j.cor + N_CORES);
					}
					else
						imprimeFrase("ARQUIVO NAO SALVO", j.fin.lin-j.ini.lin, 10, COR_Black_Red);
					s=0;
				}
				else if(a){
					a=0;
					j.ancora.lin = j.ancora.col = 0;
				}
				else{
					// se ainda nao esta inserindo um valor
					if(!e){
						t[0]='\0';
						insereTabela(&tabela, t, j.cursor.lin, j.cursor.col);
					}
					cont=0;
					e=!e;
				}
				break;
			default:
				// se esta inserindo um valor, recebeu um char valido e ainda ha espaco
				if((e || r || s) && ch!=ERR && cont < 254){
					insereNaString(t, ch, &cont);
					if(e)
						insereTabela(&tabela, t, j.cursor.lin, j.cursor.col);
				}
				break;
		}			
	}			
	encerraJanela();

	return 0;
}

void entr(int * e, int * cont){
	if(*e){
		*cont=0;
		*e=!*e;
	}
}

void insereNaString(unsigned char * t, int ch, int * cont){
	t[*cont] = ch;
	t[*cont+1] = '\0';
	++*cont;
}

void alocaCopia(CELULA ***c){
	*c = (CELULA **) malloc ((LIN_MAX+1)*sizeof(CELULA *));

	if (*c == NULL){
		perror("Erro ao alocar memoria");
		exit(1);
	}

	for (int i = 0; i < LIN_MAX+1; ++i){
		(*c)[i] = (CELULA *) malloc ((COL_MAX+1)*sizeof(CELULA));
	}
}

void copiaValores(TABELA * tab, JANELA j, CELULA **c, COORDENADAS * area, int x){
	int li, lf, ci, cf;
	calculaAreaSelecionada(j, &li, &lf, &ci, &cf);
	area->lin = lf - li;
	area->col = cf - ci;
	for (int h=0; h <= area->lin; ++h){
		for (int i = 0; i <= area->col; ++i){
			if(tab->celula[li+h][ci+i].valor != NULL){
				if(c[h][i].valor == NULL)
					c[h][i].valor = (char *) malloc(300);
				strcpy(c[h][i].valor, tab->celula[li+h][ci+i].valor);

				if(x){
					free(tab->celula[li+h][ci+i].valor);
					tab->celula[li+h][ci+i].valor = NULL;
				}
			}
			else{
				if(c[h][i].valor != NULL)
					free(c[h][i].valor);
				c[h][i].valor = NULL;
			}
		}
	}
}

void colaValores(TABELA * tab, JANELA j, CELULA **c, COORDENADAS area){
	int li, ci;
	li = j.cursor.lin;
	ci = j.cursor.col;

	for (int h=0; h <= area.lin; ++h){
		for (int i=0; i <= area.col; ++i){
			if(c[h][i].valor != NULL){
				insereTabela(tab, (unsigned char *) c[h][i].valor, li+h, ci+i);
			}
			else{
				tab->celula[li+h][ci+i].valor = NULL;
			}
		}
	}
}
