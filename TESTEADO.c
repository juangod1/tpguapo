#include <stdio.h>
#include <stdlib.h>
#include "TESTEADO.h"
#include "getnum.h"
#include <string.h>
#include <math.h>
#include <time.h>
#define BOARD_SIZE_MAX_Y 30
#define BOARD_SIZE_MAX_X 30
#define CLEAR_GRAPHICS system("clear")
#define BORRA_BUFFER while (getchar() != '\n')
#define NEWGAMEPVP 1
#define NEWGAMEPVAI 2
#define CONTINUEGAME 3

void display_Board(game_data_type *game_data);
int valid_Space(game_data_type *game_data, int i, int j);
int check_Move(game_data_type *game_data);
int check_Captures(game_data_type *game_data, int i, int j);
void fill_Blocks(game_data_type *game_data);
int count_Blocks(game_data_type *game_data);

int open_File(char *filename, game_data_type *game_data)
{
	int i,j;
	FILE * savefile;
	savefile = fopen( filename, "r");
	if(savefile==NULL){
		printf("error en la lectura del archivo");
		return 1;
	}
	fread(&game_data->mode ,sizeof(int),1, savefile);
	fread(&game_data->upnext ,sizeof(int),1, savefile);								
	fread(&game_data->size_y ,sizeof(int),1, savefile);
	fread(&game_data->size_x ,sizeof(int),1, savefile);
	for(i=0;i<BOARD_SIZE_MAX_Y;i++){
		for(j=0;j<BOARD_SIZE_MAX_Y;j++){
			fread(&game_data->board[i][j] ,sizeof(char),1, savefile);
		}
	}
	fclose (savefile);
	return 0;
}

int save_File(char *filename, game_data_type * game_data)
{
	int i,j;
	FILE * savefile;
	savefile = fopen( filename, "w");
	if(savefile==NULL){
		return 1;
	}
	fwrite(&(game_data->mode) ,sizeof(int),1, savefile);
	fwrite(&(game_data->upnext) ,sizeof(int),1, savefile);
	fwrite(&(game_data->size_y) ,sizeof(int),1, savefile);
	fwrite(&(game_data->size_x) ,sizeof(int),1, savefile);
	for(i=0;i<game_data->size_y;i++)
	{
		for(j=0;j<game_data->size_x;j++)
		{
			fwrite(&game_data->board[i][j] ,sizeof(char),1, savefile);
		}
	}
	fclose (savefile);
	return 0;
}

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

void direccion(int ang, int *i, int *j)
{
	if (ang == 0) {*j+=1;}
	else if (ang == 45)  {*j+=1; *i-=1;}
	else if (ang == 90)  {*i-=1;}
	else if (ang == 135) {*j-=1; *i-=1;}
	else if (ang == 180) {*j-=1;}
	else if (ang == 225) {*j-=1; *i+=1;}
	else if (ang == 270) {*i+=1;}
	else if (ang == 315) {*j+=1; *i+=1;}
}

void modify_Adjacent_Blocks(game_data_type *game_data)
{
    int i=(*game_data).to_x-1, j=(*game_data).to_y-1, auxj;
    i=((i<0)?(0):(i));
    auxj=((j<0)?(0):(j));
    for(i=i; i<=(*game_data).to_x+1; i++)
    {
        for(j=auxj; j<=(*game_data).to_y+1; j++)
        {
            if((*game_data).board[j][i]!=0 && j<=(*game_data).size_y && i<=(*game_data).size_x)
                (*game_data).board[j][i]=(*game_data).board[(*game_data).to_y][(*game_data).to_x];
        }
    }
}
typedef struct 
{
	int move_type;
	int from_x, from_y, to_x, to_y;
} potential_move;

int get_Move_AI(game_data_type *game_data)
{
	int i, j, k, l;
	int capt_aux, from_x_aux, from_y_aux, to_x_aux, to_y_aux, aux_i, aux_j;
	int move_type, move_index, captures=-1, equal_moves_counter=0;
	potential_move *potential_moves=NULL, *tmp;

	display_Board(game_data);

	for (i=0 ; i<(*game_data).size_y ; i++)
	{
		for (j=0 ; j<(*game_data).size_x ; j++)
		{
			if ( (*game_data).board[i][j] == 'Z' )
			{
				for ( k=0 ; k<360 ; k+=45 )
				{
					aux_i = i;
					aux_j = j;
					for ( l=0 ; l<2 ; l++ )
					{
						direccion(k, &aux_i, &aux_j);
						if ( valid_Space(game_data, aux_i, aux_j) )
						{
							move_type = check_Move(game_data);

							if (  ((capt_aux=check_Captures(game_data, aux_i, aux_j) == captures) ))
							{
								tmp = realloc(potential_moves, (++equal_moves_counter)*sizeof(potential_move));
								if (tmp) potential_moves = tmp;
								potential_moves[equal_moves_counter-1].from_x = j;
								potential_moves[equal_moves_counter-1].from_y = i;
								potential_moves[equal_moves_counter-1].to_x = aux_j;
								potential_moves[equal_moves_counter-1].to_y = aux_i;
								potential_moves[equal_moves_counter-1].move_type = move_type;
							}
							else if ( capt_aux > captures )
							{
								captures = capt_aux;
								tmp = realloc(potential_moves, sizeof(potential_move));
								if (tmp) potential_moves = tmp;
								potential_moves[0].from_x = j;
								potential_moves[0].from_y = i;
								potential_moves[0].to_x = aux_j;
								potential_moves[0].to_y = aux_i;
								potential_moves[0].move_type = move_type;
								equal_moves_counter = 1;
							}
						}
					}
				}
			}
		}
	}

	if ( equal_moves_counter == 1)
	{
		move_type = potential_moves[0].move_type;
		(*game_data).from_x = potential_moves[0].from_x;
		(*game_data).from_y = potential_moves[0].from_y;
		(*game_data).to_x = potential_moves[0].to_x;
		(*game_data).to_y = potential_moves[0].to_y;
	}
	else
	{
		move_index = rand()%(equal_moves_counter);
		move_type = potential_moves[move_index].move_type;
		(*game_data).from_x = potential_moves[move_index].from_x;
		(*game_data).from_y = potential_moves[move_index].from_y;
		(*game_data).to_x = potential_moves[move_index].to_x;
		(*game_data).to_y = potential_moves[move_index].to_y;
	}
	return move_type;
}

int check_Captures(game_data_type *game_data, int i, int j)
{
	int aux_j, aux_i, k, captures=0;
	for ( k=0 ; k<360 ; k+=45 )
	{
		aux_i = i;
		aux_j = j;
		
		direccion(k, &aux_i, &aux_j);
		if ( (*game_data).board[aux_i][aux_j]=='A' )
			captures++;
	}
	return captures;
}

void modify_Board(game_data_type *game_data, int move_type)
{
	char character = ((((*game_data).upnext)%2)?'A':'Z');
	(*game_data).board[(*game_data).to_y][(*game_data).to_x] = character;
	if (move_type==2) (*game_data).board[(*game_data).from_y][(*game_data).from_x] = 0;
	modify_Adjacent_Blocks(game_data);
}

void display_Board(game_data_type *game_data)
{
	int i,j,k=0;
	//CLEAR_GRAPHICS;
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
int initial_Turn(game_data_type *game_data)
{
	int turn=0;
	if ( (*game_data).mode != 3) /* NEWGAME */
	{
		turn=(rand()%2);
	}

	else /* CONTINUE */
	{

	}

	return turn;
}

int valid_Space(game_data_type *game_data, int i, int j)
{
	int validity=0;
	if(((*game_data).board[i][j] == 0) && (i<(*game_data).size_y) && (j<(*game_data).size_x) && (i>-1) && (j>-1))
		validity=1;
	return validity;
}

int check_Move(game_data_type *game_data)
{
	int move_type; /* 0 si es invalida, 1 si es mitosis, 2 si es salto */
	float distance, hip = ((*game_data).from_x-(*game_data).to_x)*((*game_data).from_x-(*game_data).to_x)+((*game_data).from_y-(*game_data).to_y)*((*game_data).from_y-(*game_data).to_y);

	distance = sqrt(hip);
	
	if ( (*game_data).board[(*game_data).from_y][(*game_data).from_x] == (((*game_data).upnext%2)?'A':'Z') )
	{
		if ((distance==1)||((distance<=sqrt(2)+0.05) && (distance>= sqrt(2)-0.05)))  		/* Estoy chequeando que la distancia*/ 
			move_type = 1;																	/*    sea la de un salto correcto   */
		else if ((distance==2)||((distance<=sqrt(8)+0.05) && (distance>= sqrt(8)-0.05))) 
			move_type = 2;
		else 
			move_type = 0;

		if ( !valid_Space(game_data, (*game_data).to_y, (*game_data).to_x))
			move_type = 0;
	}
	else
		move_type = 0;
	return move_type;
}

int get_Move(game_data_type *game_data)
{
    char respuesta[20], nuevalinea;
    int cantleido=0, estado=0, tipoinput=0;
    display_Board(game_data);
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

int end_Game(game_data_type *game_data)
{
	int i, j, aux_j, aux_i, l, k;
    char player= ((*game_data).upnext==1)?'A':'Z';
    
    for (i=0 ; i<(*game_data).size_y ; i++)
	{
		for (j=0 ; j<(*game_data).size_x ; j++)
		{
		    if((*game_data).board[i][j]==player)
		    {
		        aux_i=i;
		        aux_j=j;
		        for ( k=0 ; k<360 ; k+=45 )
		        {
		      		for(l=0; l<2; l++)
		      		{
		           		direccion(k, &aux_i, &aux_j);
		            	if( valid_Space(game_data, aux_i, aux_j) )
		                	return 0;
		        	}
		        }
		    }
		}
	}
	fill_Blocks(game_data);
	return count_Blocks(game_data);
}

void fill_Blocks(game_data_type *game_data)
{
	int i, j;
    char player = ((*game_data).upnext==1?'Z':'A');
    printf("%c", player);
    for (i=0 ; i<(*game_data).size_y ; i++)
	{
		for (j=0 ; j<(*game_data).size_x ; j++)
		{
		    if((*game_data).board[i][j]==0)
		        (*game_data).board[i][j]=player;
		}
	} 
}

int count_Blocks(game_data_type *game_data)
{
	int i, j, contador1=0, contador2=0;
    for (i=0 ; i<(*game_data).size_y ; i++)
	{
		for (j=0 ; j<(*game_data).size_x ; j++)
		{
		    if((*game_data).board[i][j]=='A')
		        contador1++;
		    else
		        contador2++;
		}
	}
	if(contador1==contador2)
	    return 3;
    else
        return ((contador1>contador2)?1:2);
}

void game_Loop(game_data_type *game_data)					
{
	int turn, move_type, termino=0;

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
	turn = initial_Turn(game_data);
	(*game_data).upnext = (turn%2)+1;
	while(!termino)
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

int main()
{
srand(time(NULL));
int i;

game_data_type game_data;

game_data.size_y=5;
game_data.size_x=5;
game_data.from_x=2;
game_data.from_y=2;
game_data.to_x=2;
game_data.to_y=0;
game_data.upnext=2;
game_data.mode=0;
char *aux = &game_data.board;
for(i=0; i<900; i++)
	aux[i]=0;



game_data.mode = menu(&game_data);
game_Loop(&game_data);

}
