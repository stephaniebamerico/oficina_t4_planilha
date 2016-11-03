 #include "XCLlib.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

void setFunction(char *expression, Cell *cell, Table *table)
{
	char buffer[50];
	int state = 0;
    cell->funcV.expr = (char *) malloc(sizeof(char)*(strlen(expression)+1));
    strcpy(cell->funcV.expr, expression);


    int i = 1;
    int j = 0;
    while ((i < strlen(expression)) && (expression[i]!='/') && (expression[i]!='*') && (expression[i]!='-') && (expression[i]!='+') && (state != 6))
    {
    	switch(state) {
    		case 0:
                if ((expression[i]>= 65)&&(expression[i]<=90))
                {
                    state = 2;
                    buffer[j] = expression[i];
                    j++;
                }
                else if ((expression[i]>=48)&&(expression[i]<=57))
                {
                    state = 1;
                    buffer[j] = expression[i];
                    j++;
                }
                else if (expression[i] != ' ')
                    state = 5;
            break;

            case 1:
                if((expression[i]>=48)&&(expression[i]<=57))
                {
                    state = 1;
                    buffer[j] = expression[i];
                    j++;
                }
                else if(expression[i]=='.')
                {
                    state = 4;
                    buffer[j] = expression[i];
                    j++;
                }
                else if (expression[i] != ' ')
                    state = 5;
            break;

            case 2:
                if((expression[i]>=48)&&(expression[i]<=57))
                {
                    state = 3;
                    buffer[j] = expression[i];
                    j++;
                }
                else if (expression[i] != ' ')
                    state = 5;
            break;

            case 3:
                if((expression[i]>=48)&&(expression[i]<=57))
                {
                    state = 3;
                    buffer[j] = expression[i];
                    j++;
                }
                else if (expression[i] != ' ')
                    state = 5;
            break;

            case 4:
            if((expression[i]>=48)&&(expression[i]<=57))
            {
                state = 4;
                buffer[j] = expression[i];
                j++;
            }
            else if (expression[i] != ' ')
                state = 5;
            break;
    	}

        i++;
    }

    buffer[j] = '\0';
    printf("estado: %d \n",state);
    printf("%s\n", buffer);

}

void initTable(Table *table)
{
	for (int i = 0; i < L_MAX; i++)
		for(int j = 0; j < C_MAX; j++)
        {
            table->is_function[i][j] = false;
            table->cell[i][j] = NULL;
        }
}


void setCell(char *value, Table *table, int lin,int col)
{
	if(table->cell[lin][col]!= NULL)
	{
		switch(table->cell[lin][col]->type)
		{
			case func_T:
				free(table->cell[lin][col]->funcV.expr);
			break;
			case str_T:
				free(table->cell[lin][col]->stringV);
			break;
		}
		free(table->cell[lin][col]);
	}

	int state = 0;
	table->cell[lin][col] = (Cell *) malloc(sizeof(Cell));

	for(int i = 0; i< strlen(value); i++)
	{
		switch (state) {
			case 0:
				if(value[i]=='-')
					state = 1;
				else if((value[i]>=48)&&(value[i]<=57))
					state = 2;
				else if(value[i]=='=')
					state = 5;
				else
					state = 4;
			break;
			case 1:
				if((value[i]>=48)&&(value[i]<=57))
					state = 2;
				else
					state = 4;
			break;
			case 2:
				if((value[i]>=48)&&(value[i]<=57))
					state = 2;
				else if(value[i]=='.')
					state = 3;
				else
					state = 4;
			break;
			case 3:
				if(((value[i]>=48)&&(value[i]<=57)))
					state = 3;
				else
					state = 4;
			break;
			case 4:
				state = 4;
			break;
			case 5:
				state = 5;
			break;
		}
	}

	switch (state) {
		case 1:
		case 4:
            table->is_function[lin][col] = false;
			table->cell[lin][col]->type = str_T;
			table->cell[lin][col]->stringV = (char *) malloc(sizeof(char)*(strlen(value)+1));
			strcpy(table->cell[lin][col]->stringV, value);
		break;
		case 2:
            table->is_function[lin][col] = false;
			table->cell[lin][col]->type = int_T;
			table->cell[lin][col]->intV = atoi(value);
		break;
		case 3:
            table->is_function[lin][col] = false;
			table->cell[lin][col]->type = float_T;
			table->cell[lin][col]->floatV = atof(value);
		break;
		case 5:
            table->is_function[lin][col] = true;
			table->cell[lin][col]->type = func_T;
            setFunction(value,table->cell[lin][col],table);
        break;

	}
}


//=============================================================================
//=============================================================================

void readCSV(char *filename, Table *table)
{

	initTable(table);
	char buffer[256] = "";
	FILE *CSVfile;
	CSVfile = fopen(filename, "r");

	char ch;
	int buf_count = 0, c_lin = 0, c_col = 0;
	bool quote = false;


	//arquivo aberto?
	if(CSVfile!=NULL)
	{
		//ler caracteres
		while(((ch = fgetc(CSVfile)) != EOF)&&(c_lin < L_MAX))
		{
			//pular linha
			if (ch == '\n')
			{
				if(c_lin+1<L_MAX)
				{
					if(strlen(buffer)!=0)
					{
						buffer[strlen(buffer)] = '\0';
						setCell(buffer,table,c_lin,c_col);
						memset(buffer,0,strlen(buffer));
						buf_count =0;
					}
					quote = false;
					c_lin++;
					c_col = 0;
				}
                else
                    break;
			}
			//valores com aspas
			else if (ch == '"')
			{
				quote = !quote;
			}
			//ler valor
			else if(((ch != ',')&&(ch != ' '))||quote)
			{
				//checagem para não estourar o buffer
				if(buf_count + 1 == 255)
				{
					//encerrar o buffer
					buffer[255] = '\0';
					//printf("VAI: demais\n");
					setCell(buffer,table,c_lin,c_col);
					if(c_col+1<C_MAX)
						c_col++;
					else if(c_lin+1<L_MAX)
					{
						c_lin++;
						c_col = 0;
					}
					//limpar buffer
					memset(buffer,0,strlen(buffer));
					buf_count = 0;
				}
				//inserir caracter
				buffer[buf_count] = ch;
				buf_count++;
			}
			//ignorar
			else  if (ch == ',')
			{


				//se buffer não for vazio...
				if((strlen(buffer)!=0))
				{
					buffer[strlen(buffer)] = '\0';
					//printf("VAI: normal\n");
					setCell(buffer,table,c_lin,c_col);
					if(c_col+1<C_MAX)
						c_col++;
					else if(c_lin+1<L_MAX)
					{
						c_lin++;
						c_col = 0;
					}
				}
				else
				{
					if(c_col+1<C_MAX)
						c_col++;
					else if(c_lin+1<L_MAX)
					{
						c_lin++;
						c_col = 0;
					}
				}
				memset(buffer,0,strlen(buffer));
				buf_count =0;
			}
		}
		buffer[strlen(buffer)] = '\0';
		setCell(buffer,table,c_lin,c_col);
		memset(buffer,0,strlen(buffer));
		buf_count =0;
		fclose(CSVfile);
	}
}
