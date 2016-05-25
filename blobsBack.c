#include <stdio.h>
#include <unistd.h>
#include "blobsBack.h"
#define BOARD_SIZE_Y 10
#define BOARD_SIZE_X 10
#define CLEAR_GRAPHICS for(i=0;i<50;i++) putchar('\n')

int display_board(char board[BOARD_SIZE_X][BOARD_SIZE_Y])
{
	int i,j;

	CLEAR_GRAPHICS;

	for (i=0;i<BOARD_SIZE_Y;i++)
	{
		putchar('|');
		for (j=0;j<BOARD_SIZE_X;j++)
		{
			printf("%c |",board[i][j]);
		}
		putchar('\n');
	}

}



