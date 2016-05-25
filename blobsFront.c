#include <stdio.h>
#include <stdlib.h>
#include "blobsBack.h"
#define BOARD_SIZE_MAX_Y 30
#define BOARD_SIZE_MAX_X 30
#define CLEAR_GRAPHICS for(i=0;i<50;i++) putchar('\n')
#define BORRA_BUFFER while (getchar() != '\n')

int i, j, size_x, size_y;

void display_board(char board[BOARD_SIZE_MAX_Y][BOARD_SIZE_MAX_X]);

void menu(int *opcion)
{
	int c, menu=1;
	while(menu)
	{
		CLEAR_GRAPHICS;

		if (*opcion==-1) printf("\nError al leer parametros.\n");

		printf("1. Juego de dos jugadores\n2. Juego contra computadora\n3.Recuperar un juego grabado\n4. Terminar\n\nElegir opcion: ");
		BORRA_BUFFER;
		scanf("%i", opcion);

		if ((c=gechar())!='\n') *opcion = -1;
		else if (*opcion == 1) 
	}
}

void display_board(char board[BOARD_SIZE_MAX_Y][BOARD_SIZE_MAX_X])
{/* BORRAR COMENTARIO ANTES DE ENTREGA 
El tablero lo vamos a definir siempre con el tamaño maximo (30) para evitar conflicto de norma IH, recibimos como parametro el tamaño verdadero. */
	
	CLEAR_GRAPHICS;

	for ( i=0 ; i<size_y ; i++ )
	{
		putchar('|');
		for ( j=0 ; j<size_x ; j++ )
		{
			printf("%c |", board[i][j]);
		}
		putchar('\n');
	}

}

main(){

}