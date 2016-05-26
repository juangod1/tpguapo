#include <stdio.h>
#include "blobsBack.h"
#include <stdlib.h>
#include <time.h>
#define BOARD_SIZE_MAX_Y 30
#define BOARD_SIZE_MAX_X 30

int check_move(const char *move) /* recibe movida de get_move() y chequea si es valida */
{

}

void modify_board(const char *move, char board[][])
{

}

int turn(int mode)
{
	if ( mode != 3) /* NEWGAME */
	{
		srand(time(NULL));
		return rand() % 2;
	}

	else /* CONTINUE */
	{
		
	}
}