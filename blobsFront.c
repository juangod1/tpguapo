#include <stdio.h>
#include <stdlib.h>
#include "blobsBack.h"
#include "getnum.h"
#include <string.h>
#include <time.h>
#define BOARD_SIZE_MAX_Y 30
#define BOARD_SIZE_MAX_X 30
#define CLEAR_GRAPHICS system("clear")
#define BORRA_BUFFER while (getchar() != '\n')
#define NEWGAMEPVP 1
#define NEWGAMEPVAI 2
#define CONTINUEGAME 3

char* menu(game_data_type *game_data)
{
	int menu_state=1, opcion=0, cantleido;
	char file_stdin[20], nuevalinea;
	char * file = malloc(17*sizeof(char));
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
				(*game_data).size_y = getint("Ingrese la cantidad de filas (Entre 5 y 30): ");
				(*game_data).size_x = getint("Ingrese la cantidad de columnas (Entre 5 y 30): ");
				if (((*game_data).size_y>30)||((*game_data).size_y<5)||((*game_data).size_x<5)||((*game_data).size_x>30))
					opcion = -1;
				else
					menu_state = 0;	
				break;	
			case 3:
				printf("Ingrese el nombre del archivo (max 15 caracteres): ");
				fgets(file_stdin, 17, stdin);
            	if((cantleido=(sscanf(file_stdin, "%s%c", file, &nuevalinea)))==2 && nuevalinea=='\n')
            	{
					if (open_File(file,game_data)==0) 
					{	
						menu_state = 0;	
					}
				}
				else {
					printf("Error al cargar (El archivo esta corrupto o no existe)\n");
					opcion = -1;
					}
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
	(*game_data).mode = opcion;
	if ( opcion == 3)
		return file;
	else 
		return NULL;
}

void display_Board(game_data_type *game_data)
{
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

int get_Move(game_data_type *game_data)
{
    char respuesta[20], nuevalinea;
    int cantleido=0, estado=0, tipoinput=0;
    char *filename=malloc(15*sizeof(char));
    while(tipoinput==0)
        {
            printf("Turno de Jugador %d\n", (*game_data).upnext);
            fgets(respuesta, 21, stdin);
            if((cantleido=(sscanf(respuesta, "save %s%c", filename, &nuevalinea)))==2 && nuevalinea=='\n')
                	estado=1;
            if(strcmp("exit\n",respuesta)==0)
                estado=2;
            switch(estado)
            {
                case 1:
                if (save_File(filename,game_data)==1) 
				{
					printf("Error al guardar)\n");
					
				}
				else 
					save_File(filename,game_data);

                    tipoinput=3;
                    break;
                case 2:
                    tipoinput=4;
                    break;

                default:
                    cantleido=sscanf(respuesta, "[%d,%d] [%d,%d]%c", &(*game_data).from_x, &(*game_data).from_y, &(*game_data).to_x, &(*game_data).to_y, &nuevalinea);
                    if(cantleido==5 && nuevalinea==10)
                    {
                        if((tipoinput=check_Move(game_data))==0)
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


void game_Loop(game_data_type *game_data, char *archivo)					
{
	int move_type, cargo=0, guardado=0, termino=0;

	switch((*game_data).mode)
	{
		case NEWGAMEPVP ... NEWGAMEPVAI:
			(*game_data).board[0][0]= 'A';
			(*game_data).board[(*game_data).size_y-1][0]= 'A';
			(*game_data).board[0][(*game_data).size_x-1]= 'Z';
			(*game_data).board[(*game_data).size_y-1][(*game_data).size_x-1]= 'Z';	
			break;
		case CONTINUEGAME:
			cargo=1;
			(*game_data).upnext--;
			open_File(archivo, game_data);
			break;
		
	}
	if(cargo==0)
	{
		(*game_data).upnext = initial_Turn();
		(*game_data).upnext = (((*game_data).upnext)%2)+1;
	}
	while(!termino)
	{
		display_Board(game_data);
		if(guardado==1)
		{
			guardado=0;
			printf("El archivo fue guardado con exito, oprima 'enter' para continuar.\n");
			if(getchar()!='\n')
				BORRA_BUFFER;
		}
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
			(*game_data).upnext= (((*game_data).upnext)%2)+1;
			break;
		case 3:
			guardado=1;
			break;
		case 4:
			CLEAR_GRAPHICS;
			exit(0);
			break;
		}
		display_Board(game_data);
		termino=end_Game(game_data);
	}
	system("sleep 1");
	display_Board(game_data);
	if(termino==3)
	    printf("El juego ha terminado en un empate!");
	else
	    printf("Felicitaciones jugador %d has ganado! \n",termino);
	printf("Oprima 'enter' para salir\n");
	getchar();
	CLEAR_GRAPHICS;
}

int main()
{
	int i;

	srand(time(NULL));
	game_data_type game_data = {0};

	char *archivo = menu(&game_data);
	game_Loop(&game_data,archivo);

}
