#include <stdio.h>
#include "blobsBack.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define BOARD_SIZE_MAX_Y 30
#define BOARD_SIZE_MAX_X 30

int valid_space(char board[][BOARD_SIZE_MAX_X], int i, int j)
{
	int validity=0;
	if((board[i][j] == 0) && (i<size_y) && (j<size_x) && (i>-1) && (j>-1));
		validity=1;
	return validity;
}

void modifyAdjacentBlocks(char board[30][30])
{
    int i=to_x-1, j=to_y-1, auxj;
    i=((i<0)?(0):(i));
    auxj=((j<0)?(0):(j));
    for(i=i; i<=to_x+1; i++)
    {
        for(j=auxj; j<=to_y+1; j++)
        {
            if(board[j][i]!=0 && j<=size_y && i<=size_x)
                board[j][i]=board[to_y][to_x];
        }
    }
}

int check_move(int from_x, int from_y, int to_x, int to_y, char board[30][30], int upnext)
{
	int move_type; /* 0 si es invalida, 1 si es mitosis, 2 si es salto */
	float distance, hip = (from_x-to_x)*(from_x-to_x)+(from_y-to_y)*(from_y-to_y);

	distance = sqrt(hip);
	if ( board[from_y][from_x] == ((upnext%2)?'A':'Z') )
	{
		if ((distance==1)||((distance<=sqrt(2)+0.05) && (distance>= sqrt(2)-0.05)))  		/* Estoy chequeando que la distancia*/ 
			move_type = 1;																	/*    sea la de un salto correcto   */
		else if ((distance==2)||((distance<=sqrt(8)+0.05) && (distance>= sqrt(8)-0.05))) 
			move_type = 2;
		else 
			move_type = 0;

		if ( !valid_space(board,to_y,to_x) ) move_type = 0;
	}
	else 
		move_type = 0;
	return move_type;
}

int init_turn(int mode)
{
	int turn;
	if ( mode != 3) /* NEWGAME */
	{
		srand(time(NULL));
		turn = rand() % 2;
	}

	else /* CONTINUE */
	{

	}

	return turn;
}

int direccion(int ang, int *i, int *j)
{
	if (ang == 0) j++;
	else if (ang == 45)  {*j+=1; *i-=1;}
	else if (ang == 90)  {*i-=1;}
	else if (ang == 135) {*j-=1; *i-=1;}
	else if (ang == 180) {*j-=1;}
	else if (ang == 225) {*j-=1; *i-=1;}
	else if (ang == 270) {*i-=1;}
	else if (ang == 315) {*j+=1; *i-=1;}
}


int endgame(char board[BOARD_SIZE_MAX_Y][BOARD_SIZE_MAX_X], int upnext)
{
	int i, j, k, l, aux_i, aux_j;
	char target = ((upnext==1)?'A':'Z');

	for (i=0 ; i<size_y ; i++)
	{
		for (j=0 ; j<size_x ; j++)
		{
			if ( board[i][j] == target )
			{
				for ( k=0 ; k<360 ; k+=45 )
				{
					aux_i = i;
					aux_j = j;
					for ( l=0 ; l<2 ; l++ )
					{
						direccion(k, &i, &j);
						if ( valid_space(board, i, j) ) 
							return 0;
					}
					i = aux_i;
					j = aux_j;
				}
			}
		}
	}
	return 1;
}

int check_captures(int i, int j, char board[][BOARD_SIZE_MAX_X])
{
	int aux_j, aux_i, k, captures=0;
	for ( k=0 ; k<360 ; k+=45 )
	{
		aux_i = i;
		aux_j = j;
		
		direccion(k, &i, &j);
		if ( board[i][j]=='A' )
			captures++;
		
		i = aux_i;
		j = aux_j;
	}
	return captures;
}

typedef struct 
{
	int from_x, from_y;
} potential_moves;

int getmove_ai(char board[][BOARD_SIZE_MAX_X])
{
	int i, j, k, l;
	int capt_aux, from_x_aux, from_y_aux, to_x_aux, to_y_aux, aux_i, aux_j;
	int move_type, captures=0;

	for (i=0 ; i<size_y ; i++)
	{
		for (j=0 ; j<size_x ; j++)
		{
			if ( board[i][j] == 'Z' )
			{
				for ( k=0 ; k<360 ; k+=45 )
				{
					aux_i = i;
					aux_j = j;
					for ( l=0 ; l<2 ; l++ )
					{
						direccion(k, &i, &j);
						if ( valid_space(board, i, j) )
						{
							move_type = check_move(aux_j, aux_i, j, i, board, 2);

							if ( (capt_aux=check_captures(i ,j, board)) == captures)
							{

							}
							else if ( capt_aux > captures )
							{
								captures = capt_aux;
							}
						}
					}
					i = aux_i;
					j = aux_j;
				}
			}
		}
	}
	return move_type;
}

int openfile(char *filename, int mode,int upnext,int size_y, int size_x,char board[30][30])
{
	/* Necesito declarar todos los parametros de entrada? o por ser golbales no es necesario? */
	FILE * savefile;
	savefile = fopen( filename, "r");
	if(savefile==NULL){
		printf("error en la lectura del archivo");
		return 1;
	}
	fread(&mode ,sizeof(int),1, savefile);
	fread(&upnext ,sizeof(int),1, savefile);								
	fread(&size_y ,sizeof(int),1, savefile);
	fread(&size_x ,sizeof(int),1, savefile);
	fread(board ,sizeof(char),900, savefile);
	fclose (savefile);
	return 0;
}
int savefile(char *filename, int mode,int upnext,int size_y, int size_x,char board[30][30])
{
	/* Necesito declarar todos los parametros de entrada? o por ser golbales no es necesario? */
	FILE * savefile;
	savefile = fopen( filename, "w");
	if(savefile==NULL){
		printf("no se pudo crear el archivo");
		return 1;
	}
	fwrite(&mode ,sizeof(int),1, savefile);
	fwrite(&upnext ,sizeof(int),1, savefile);
	fwrite(&size_y ,sizeof(int),1, savefile);
	fwrite(&size_x ,sizeof(int),1, savefile);
	fwrite(board ,sizeof(char),900, savefile);
	fclose (savefile);
	return 0;
}
