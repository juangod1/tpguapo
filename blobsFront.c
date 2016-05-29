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

void display_board(char board[BOARD_SIZE_MAX_Y][BOARD_SIZE_MAX_X]);

void modify_board(char board[BOARD_SIZE_MAX_Y][BOARD_SIZE_MAX_X], int move_type, int upnext);

int size_x, size_y, from_x, from_y, to_x, to_y;

int menu()
{
	int menu_state=1, opcion=0;
	char file[]={};
	while(menu_state)
	{
		CLEAR_GRAPHICS;

		if (opcion==-1) 
			printf("\nError al leer parametros.\n");


		opcion = getint("\n1. Juego de dos jugadores\n2. Juego contra computadora\n3. Recuperar un juego grabado\n4. Terminar\n\nElega la opcion correspondiente: ");
		
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

int getmove(int upnext, char board[BOARD_SIZE_MAX_Y][BOARD_SIZE_MAX_X])
{
	int jugada=0;
	display_board(board);
	printf("Jugador %d escriba su próxima jugada\n",upnext);
	while(!jugada)
	{
		scanf("[%d,%d][%d,%d]", &from_x, &from_y, &to_x, &to_y);
		while(getchar()!='\n')
		{
			BORRA_BUFFER;
			display_board(board);
			printf("Error al leer los parámetros. Intente nuevamente\n");
			scanf("[%d,%d][%d,%d]", &from_x, &from_y, &to_x, &to_y);
		}
		if(!(jugada=(check_move(from_x, from_y, to_x, to_y, board, upnext))))
		{
			display_board(board);
			printf("Error: Jugada Imposible. Intente nuevamente\n");
		}
	}
	return jugada;
}


void display_board(char board[BOARD_SIZE_MAX_Y][BOARD_SIZE_MAX_X])
{/* BORRAR COMENTARIO ANTES DE ENTREGA 
El tablero lo vamos a definir siempre con el tamaño maximo (30) para evitar conflicto de norma IH, tamaño verdadero es una variable global (size_y , size_x) */	
	int i,j;
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

void modify_board(char board[BOARD_SIZE_MAX_Y][BOARD_SIZE_MAX_X], int move_type, int upnext)
{
	char character = ((upnext%2)?'A':'Z');
	board[to_y][to_x] = character;
	if (move_type==2)
		board[from_y][from_x] = 0;
}

void game_loop(int mode)
{
	int upnext, turn;
	char board[BOARD_SIZE_MAX_X][BOARD_SIZE_MAX_Y]={};
	switch(mode)
	{
		case NEWGAMEPVP ... NEWGAMEPVAI:
			board[0][0]= 'A';
			board[size_y-1][0]= 'A';
			board[0][size_x-1]= 'Z';
			board[size_y-1][size_x-1]= 'Z';	
			break;
		case CONTINUEGAME:
			break;
		
	}
	turn = init_turn(mode);
	upnext = (turn%2)+1;
	while(!endgame(board, upnext))
	{
		int move_type = getmove(upnext, board);
		modify_board(board, move_type, upnext);
		upnext = (++turn%2)+1;
	}
}

int main()
{
int option = menu();
game_loop(option);

}