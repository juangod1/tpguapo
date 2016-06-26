#include <stdio.h>
#include <stdlib.h>
#include "blobsBack.h"
#include <string.h>
#include <time.h>
#define BOARD_SIZE_MAX_Y 30
#define BOARD_SIZE_MAX_X 30
#define BORRA_BUFFER while (getchar() != '\n')
#define NEWGAMEPVP 1
#define NEWGAMEPVAI 2
#define CONTINUEGAME 3

char* menu(game_data_type *game_data)
{
	int menu_state=1, opcion=0, cantleido, inputcorrecto=0;
	char file_stdin[20], nuevalinea, size_string[10], respuesta[3];
	char * file = malloc(17*sizeof(char));
	while(menu_state)
	{
		printf("\n1. Juego de dos jugadores\n2. Juego contra computadora\n3. Recuperar un juego grabado\n4. Terminar\n\nElija la opcion correspondiente: ");
		fgets(respuesta, 2, stdin);
		sscanf(respuesta, "%d", &opcion);
		if(getchar()!='\n')
		{
			opcion=-1;
			BORRA_BUFFER;
		}
		switch(opcion){
			case 1 ... 2:
				while(inputcorrecto==0)
				{
					while(inputcorrecto==0)
					{
						printf("Ingrese la cantidad de filas (Entre 5 y 30): \n");
						fgets(size_string, 4, stdin);
						cantleido = sscanf(size_string, "%i%c", &((*game_data).size_y), &nuevalinea);
						if ((nuevalinea != '\n') || (cantleido != 2) || (*game_data).size_y>30 || (*game_data).size_y<5)
						{
							printf("Porfavor ingrese un numero entre 5 y 30\n");
						}
						else
							inputcorrecto=1;
						nuevalinea=0;
					}
					inputcorrecto=0;
					while(inputcorrecto==0)
					{
						printf("Ingrese la cantidad de columnas (Entre 5 y 30): \n");
						fgets(size_string, 4, stdin);
						cantleido = sscanf(size_string, "%i%c", &((*game_data).size_x), &nuevalinea);
						if ((nuevalinea != '\n') || (cantleido != 2) || (*game_data).size_x>30 || (*game_data).size_x<5)
						{
							printf("Porfavor ingrese un numero entre 5 y 30\n");
						}
						else
							inputcorrecto=1;
						nuevalinea=0;
					}
					menu_state = 0;
					inputcorrecto=1;
				}
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
				}
				break;
			case 4:
				exit(0);
				break;
			default:
				printf("Porfavor ingrese una opcion correcta");
				break;
			}
	}
	(*game_data).mode = opcion;
	if ( opcion == 3)
		return file;
	else{
		free(file);
		return NULL;
	}
}

void display_Board(game_data_type *game_data)
{
	int i,j,k=0;
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
	for (i=0;i!=(*game_data).size_x;i++)
		{
			if (i<9) printf("%d ",i);
			else printf("%d",i);
		}
	putchar('\n');
}

int get_Move(game_data_type *game_data)
{
    char respuesta[20], save[5], nuevalinea;
    int cantleido=0, estado=0, tipoinput=0, saveo=1, yesorno;
    char *filename=calloc(15,15);
    while(tipoinput==0)
		{
  		printf("Turno de Jugador %d\n", (*game_data).upnext);
	 		fgets(respuesta, 21, stdin);
    	if((cantleido=(sscanf(respuesta, "%s %s%c", save, filename, &nuevalinea)))==3 && nuevalinea=='\n')
    	{
  			if(!strcmp(save, "save"))
   				estado=1;
    	}
      if(strcmp("quit\n",respuesta)==0)
      	estado=2;
      switch(estado)
        {
            case 1:
              if (save_File(filename,game_data)==1)
							{
								printf("Error al guardar)\n");
							}
							else
							{
								save_File(filename,game_data);
              	tipoinput=3;
              }
	            break;
            case 2:
            	printf("Desea Guardar? y/n\n");
            	while(((yesorno=getchar())!='y' && yesorno!='n') || getchar()!='\n')
            	{
            		if(getchar()!='\n')
            		BORRA_BUFFER;
            		printf("Porfavor ingrese y o n.\n");
            	}
            	if(yesorno=='y')
            	{
            		while(saveo==1)
            		{
            			printf("Escriba el nombre deseado.\n");
            			fgets(respuesta, 15, stdin);
									sscanf(respuesta, "%s%c", filename, &nuevalinea);
									if (nuevalinea=='\n')
									{
										if ((saveo=save_File(filename,game_data))==1)
										{
											printf("Error al guardar\n");
										}
										else
										{
											save_File(filename,game_data);
											printf("Guardado con exito!\n");
               		 		tipoinput=4;
            				}
									}
									else
										printf("No se admiten espacios\n");
                }
              }
            	else
              	tipoinput=4;
              break;
            default:
                cantleido=sscanf(respuesta, "[%d,%d] [%d,%d]%c", &(*game_data).from_y, &(*game_data).from_x, &(*game_data).to_y, &(*game_data).to_x, &nuevalinea);
                if(cantleido==5 && nuevalinea==10)
                {
                    if((tipoinput=check_Move(game_data))==0)
                    {
          	          printf("Error: Jugada Imposible\n");
          	        }
                }
           	    else
								{
        	        printf("Error: Lectura de parametros incorrectos\n");
          			}
      	}
   	}
	free(filename);
  return tipoinput;
}


void game_Loop(game_data_type *game_data, char *archivo)
{
	int move_type, cargo=0, guardado=0, termino=0, captures;

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
	free(archivo);
	if(cargo==0)
	{
		(*game_data).upnext = initial_Turn();
		(*game_data).upnext = (((*game_data).upnext)%2)+1;
	}
	display_Board(game_data);
	while(!termino)
	{
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
		}
		else
			move_type = get_Move(game_data);
		switch(move_type)
		{
		case 1 ... 2:
			captures = check_Captures(game_data, (*game_data).to_y, (*game_data).to_x);
			modify_Board(game_data, move_type);
			if ((*game_data).upnext==1)
			{
				(*game_data).blobsA += captures;
				(*game_data).blobsZ -= captures;
				if (move_type==1)
					(*game_data).blobsA++;
			}
			else
			{
				(*game_data).blobsA -= captures;
				(*game_data).blobsZ += captures;
				if (move_type==1)
					(*game_data).blobsZ++;
			}

			(*game_data).upnext= (((*game_data).upnext)%2)+1;
			break;
		case 3:
			guardado=1;
			break;
		case 4:
			exit(0);
			break;
		}
		display_Board(game_data);
		termino=end_Game(game_data);
	}
	display_Board(game_data);
	if(termino==3)
	    printf("El juego ha terminado en un empate!");
	else
	    printf("Felicitaciones jugador %d has ganado! \n",termino);
	printf("Oprima 'enter' para salir\n");
	getchar();
}

void main()
{

	srand(time(NULL));
	game_data_type game_data = {0};
	game_data.blobsA = 2;
	game_data.blobsZ = 2;

	char *archivo = menu(&game_data);
	game_Loop(&game_data,archivo);

}
