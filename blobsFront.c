#include <stdio.h>
#include <stdlib.h>
#include "blobsBack.h"
#include "getnum.h"
#include <string.h>
#define BOARD_SIZE_MAX_Y 30
#define BOARD_SIZE_MAX_X 30
#define CLEAR_GRAPHICS system("clear")
#define BORRA_BUFFER while (getchar() != '\n')
#define NEWGAMEPVP 1
#define NEWGAMEPVAI 2
#define CONTINUEGAME 3

void display_Board(game_data_type *game_data);

int menu(game_data_type *game_data)
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
				(*game_data).size_y = 4;
				printf("%d", (*game_data).size_y);
				(*game_data).size_y = getint("Ingrese la cantidad de filas (Entre 5 y 30): ");
				(*game_data).size_x = getint("Ingrese la cantidad de columnas (Entre 5 y 30): ");
				if (((*game_data).size_y>30)||((*game_data).size_y<5)||((*game_data).size_x<5)||((*game_data).size_x>30))
					opcion = -1;
				else
					menu_state = 0;	
				break;	
			case 3:
				printf("Ingrese el nombre del archivo: ");
				scanf("%s",file);
				open_file(file);
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
	return (*game_data).mode = opcion;
}

int get_Move(game_data_type *game_data)
{
    char respuesta[20], nuevalinea;
    int cantleido=0, estado=0, tipoinput=0;
    display_Board( game_data );
    char *filename=malloc(15*sizeof(char));
    while(tipoinput==0)
        {
            printf("Turno de Jugador %d\n", (*game_data).upnext);
            fgets(respuesta, 15, stdin);
            if((cantleido=(sscanf(respuesta, "save %s%c", filename, &nuevalinea)))==2 && nuevalinea=='\n')
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
                    cantleido=sscanf(respuesta, "[%d,%d] [%d,%d]%c", &(*game_data).from_x, &(*game_data).from_y, &(*game_data).to_x, &(*game_data).to_y, &nuevalinea);
                    if(cantleido==5 && nuevalinea==10)
                    {
                        if((tipoinput=check_Move((*game_data).from_x, (*game_data).from_y, (*game_data).to_x, (*game_data).to_y, (*game_data).board, (*game_data).upnext))==0)
                        {
                            display_Board( game_data );
                            printf("Error: Jugada Imposible\n");
                        }
                    }
                    else
                    {
                    	display_Board( game_data );
                        printf("Error: Lectura de parametros incorrectos\n");
           			}
           	}
        }
        return tipoinput;
}


void display_Board(game_data_type *game_data)
{/* BORRAR COMENTARIO ANTES DE ENTREGA 
El tablero lo vamos a definir siempre con el tamaño maximo (30) para evitar conflicto de norma IH, tamaño verdadero es una variable global (size_y , size_x) */	
	int i,j,k=0;
	CLEAR_GRAPHICS;
	putchar('\n');
	for ( i=0 ; i<(*game_data).size_y ; i++ )
	{
		if (i<10) printf("%d ",k++);
		else printf("%d",k++);
		putchar('|');
		for ( j=0 ; j<(*game_data).size_x ; j++ )
		{
			if ((*game_data).board[i][j] == 0)
				printf("%c |", (*game_data).board[i][j]);
			else 
				printf("%c|", (*game_data).board[i][j]);
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

void game_Loop(game_data_type *game_data)
{
	int turn, move_type;

	switch((*game_data).mode)
	{
		case NEWGAMEPVP ... NEWGAMEPVAI:
			(*game_data).board[0][0]= 'A';
			(*game_data).board[(*game_data).size_y-1][0]= 'A';
			(*game_data).board[0][(*game_data).size_x-1]= 'Z';
			(*game_data).board[(*game_data).size_y-1][(*game_data).size_x-1]= 'Z';	
			break;
		case CONTINUEGAME:
			break;
		
	}
	turn = initial_Turn((*game_data).mode);
	(*game_data).upnext = (turn%2)+1;
	while(!end_Game((*game_data).board, (*game_data).upnext, (*game_data).size_y, (*game_data).size_x))
	{
		if (((*game_data).mode==2)&&((*game_data).upnext==2))
		{
			move_type = get_Move_AI(game_data);
			system("sleep 1");
		}
		else
			move_type = get_Move(game_data);
		switch(move_type)
		{
		case 1 ... 2: 
			modify_Board(game_data, move_type);
			(*game_data).upnext = (++turn%2)+1;
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
	printf("Felicitaciones jugador %d has ganado! \n",((*game_data).upnext%2+1));
	printf("pulse ctrl+z para salir\n");
	while(1);
}

int main()
{

game_data_type *game_data;
game_data = malloc(sizeof(game_data_type));
*game_data = {};
game_data->mode = menu(game_data);
game_Loop(game_data);
}
