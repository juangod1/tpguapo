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

		printf("\n1. Juego de dos jugadores\n2. Juego contra computadora\n3. Recuperar un juego grabado\n4. Terminar\n\nElija la opcion correspondiente: ");		
		scanf("%d", &opcion);
		if(getchar()!='\n')
		{
			opcion=-1;
			BORRA_BUFFER;
		}
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

int getmove(int upnext, char board[][BOARD_SIZE_MAX_X])
{
    char respuesta[20], nuevalinea;
    int cantleido=0, estado=0, tipoinput=0;
    display_board(board);
    while(tipoinput==0)
        {
            printf("Turno de Jugador %d\n", upnext);
            fgets(respuesta, 15, stdin);
            if(strcmp("save\n",respuesta)==0)
                estado=1;
            if(strcmp("exit\n",respuesta)==0)
                estado=2;
            switch(estado)
            {
                case 1:
                    tipoinput=3;
                    break;
                case 2:
                    tipoinput=4;
                    break;
                default:
                    cantleido=sscanf(respuesta, "[%d,%d] [%d,%d]%c", &from_x, &from_y, &to_x, &to_y, &nuevalinea);
                    if(cantleido==5 && nuevalinea==10)
                    {
                        if((tipoinput=check_move(from_x, from_y, to_x, to_y, board, upnext))==0)
                        {
                            display_board(board);
                            printf("Error: Jugada Imposible\n");
                        }
                    }
                    else
                    {
                    	display_board(board);
                        printf("Error: Lectura de parametros incorrectos\n");
           			}
           	}
        }
        return tipoinput;
}


void display_board(char board[BOARD_SIZE_MAX_Y][BOARD_SIZE_MAX_X])
{/* BORRAR COMENTARIO ANTES DE ENTREGA 
El tablero lo vamos a definir siempre con el tamaño maximo (30) para evitar conflicto de norma IH, tamaño verdadero es una variable global (size_y , size_x) */	
	int i,j,k=0;
	CLEAR_GRAPHICS;

	for ( i=0 ; i<size_y ; i++ )
	{
		if (i<10) printf("%d ",k++);
		else printf("%d",k++);
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
	printf("   ");
	for (i=0;i!=k;i++)
		{
			if (i<10) printf("%d ",i);
			else printf("%d",i);
		}
	putchar('\n');
}

void modify_board(char board[BOARD_SIZE_MAX_Y][BOARD_SIZE_MAX_X], int move_type, int upnext)
{
	char character = ((upnext%2)?'A':'Z');
	board[to_y][to_x] = character;
	if (move_type==2)
		board[from_y][from_x] = 0;
	modifyAdjacentBlocks(board);
}

void game_loop(int mode)
{
	int upnext, turn, move_type;
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
		/*if ((mode==2)&&(upnext==2))
			move_type = getmove_ai(board[][BOARD_SIZE_MAX_X]);
		else*/
		move_type = getmove(upnext, board);
		switch(move_type)
		{
		case 1 ... 2: 
			modify_board(board, move_type, upnext);
			upnext = (++turn%2)+1;
			break;
		case 3:
			printf("saving...\n");
			break;
		case 4:
			CLEAR_GRAPHICS;
			exit(0);
			break;
		}
	}
	printf("Felicitaciones jugador %d has ganado! \n",(upnext%2+1));
	printf("pulse ctrl+z para salir\n");
	while(1)
	{
	    
	}
}

int main()
{
int option = menu();
game_loop(option);

}
