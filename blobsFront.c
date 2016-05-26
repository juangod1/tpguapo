#include <stdio.h>
#include <stdlib.h>
#include "blobsBack.h"
#include "getnum.h"
#define BOARD_SIZE_MAX_Y 30
#define BOARD_SIZE_MAX_X 30
#define CLEAR_GRAPHICS system("clear")
#define BORRA_BUFFER while (getchar() != '\n')
#define NEWGAMEPVP 1
#define NEWGAMEPVAI 2
#define CONTINUEGAME 3

int i, j, size_x, size_y;

void display_board(char board[BOARD_SIZE_MAX_Y][BOARD_SIZE_MAX_X]);

int menu()
{
	int menu_state=1, opcion=0;
	char file[]={};
	while(menu_state)
	{
		CLEAR_GRAPHICS;

		if (opcion==-1) 
			printf("\nError al leer parametros.\n");


		opcion = getint("\n1. Juego de dos jugadores\n2. Juego contra computadora\n3. Recuperar un juego grabado\n4. Terminar\n\nElegir opcion: ");
		
		switch(opcion){
			case 1 ... 2:
				size_y = getint("Ingrese la cantidad de filas (Entre 5 y 30): ");
				size_x = getint("Ingrese la cantidad de columnas (Entre 5 y 30): ");
				if ((size_y>30)||(size_y<5)||(size_x<5)||(size_x>30))
					opcion = -1;
				else
					menu_state = 0;	
				break;	
			case 3:
				printf("Ingrese el nombre del archivo: ");
				scanf("%s",file);
				if (getchar()!='\n') /* || (NO ENCUENTRA EL ARCHIVO) no se como pija lo vamos a hacer */ 
				{
					printf("Error al cargar (El archivo esta corrupto o no existe)\n");
					opcion = -1;
				}
				else 
					menu_state = 0;	
				break;
			case 4:
				CLEAR_GRAPHICS;
				exit(0);
				break;
			default:
				opcion = -1;
				break;
			}
	}
	return opcion;
}

void display_board(char board[BOARD_SIZE_MAX_Y][BOARD_SIZE_MAX_X])
{/* BORRAR COMENTARIO ANTES DE ENTREGA 
El tablero lo vamos a definir siempre con el tamaño maximo (30) para evitar conflicto de norma IH, tamaño verdadero es una variable global (size_y , size_x) */
	
	CLEAR_GRAPHICS;

	for ( i=0 ; i<size_y ; i++ )
	{
		putchar('|');
		for ( j=0 ; j<size_x ; j++ )
		{
			if (board[i][j] == 0)
				printf("%c |", board[i][j]);
			else 
				printf("%c|", board[i][j]);
		}
		putchar('\n');
	}
}

void game_loop(int mode){
	char board[BOARD_SIZE_MAX_X][BOARD_SIZE_MAX_Y]={};
	while(1){
		switch(mode){
			case NEWGAMEPVP ... NEWGAMEPVAI:
				board[0][0]= 'A';
				board[size_y-1][0]= 'A';
				board[0][size_x-1]= 'Z';
				board[size_y-1][size_x-1]= 'Z';
				display_board(board);
				break;
			case CONTINUEGAME:
				
				break;
		}
	}
}

int main()
{
int option=menu();
game_loop(option);
}