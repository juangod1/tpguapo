#include <stdio.h>
#include "blobsBack.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define BOARD_SIZE_MAX_Y 30
#define BOARD_SIZE_MAX_X 30

int check_move(int from_x, int from_y, int to_x, int to_y, char board[30][30], int upnext)
{
	int move_type; /* 0 si es invalida, 1 si es mitosis, 2 si es salto */
	float distance, hip = (from_x-to_x)*(from_x-to_x)+(from_y-to_y)*(from_y-to_y);

	distance = sqrt(hip);
	if ( board[from_y][from_x] == (upnext)?'Z':'A' )
	{
		if (distance==1) 
			move_type = 1;
		else if (distance==2) 
			move_type = 2;
		else 
			move_type = 0;

		if ( !valid_space(board,to_y,to_x) ) move_type = 0;
	}
	else 
		move_type = 0;
	return move_type; 
}

void modify_board(const char *move, char board[BOARD_SIZE_MAX_Y][BOARD_SIZE_MAX_X])
{

}

int turn(int mode)
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
	else if (ang == 45)  {*j+=1; (*i)-=1;}
	else if (ang == 90)  {*i-=1;}
	else if (ang == 135) {*j-=1; *i-=1;}
	else if (ang == 180) {*j-=1;}
	else if (ang == 225) {*j-=1; *i-=1;}
	else if (ang == 270) {*i-=1;}
	else if (ang == 315) {*j+=1; *i-=1;}
}

int valid_space(char board[][BOARD_SIZE_MAX_X], int i, int j)
{
	int validity=0;
	if((board[i][j] == 0) && (i<size_y) && (j<size_x) && (i>-1) && (j>-1));
		validity=1;
	return validity;
}

int endgame(char board[BOARD_SIZE_MAX_Y][BOARD_SIZE_MAX_X], int upnext)
{
	int i, j, k, l, aux_i, aux_j;
	upnext = upnext%2;
	char target = (upnext?'Z':'A');

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