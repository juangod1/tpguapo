#include <stdio.h>
#include "blobsBack.h"
#include <stdlib.h>
#include <time.h>
#define BOARD_SIZE_MAX_Y 30
#define BOARD_SIZE_MAX_X 30

int check_move(const char *move) /* recibe movida de get_move() y chequea si es valida */
{

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

int endgame(char board[BOARD_SIZE_MAX_Y][BOARD_SIZE_MAX_X])
{

}