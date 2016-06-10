#include <stdio.h>
#include <stdlib.h>
#include "blobsBack.h"
#include "getnum.h"
#include <math.h>
#define BOARD_SIZE_MAX_Y 30
#define BOARD_SIZE_MAX_X 30

int valid_Space(game_data_type *game_data, int i, int j);
int check_Captures(game_data_type *game_data, int i, int j);
void fill_Blocks(game_data_type *game_data);
int count_Blocks(game_data_type *game_data);

int open_File(char *filename, game_data_type *game_data)
{
	int i,j;
	FILE * savefile;
	savefile = fopen( filename, "r");
	if(savefile==NULL){
		return 1;
	}
	fread(&game_data->mode ,sizeof(int),1, savefile);
	fread(&game_data->upnext ,sizeof(int),1, savefile);								
	fread(&game_data->size_y ,sizeof(int),1, savefile);
	fread(&game_data->size_x ,sizeof(int),1, savefile);
	for(i=0;i<(game_data->size_y);i++)
	{
	
		fread(game_data->board[i] ,game_data->size_x,1, savefile);
	
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
	for(i=0;i<(game_data->size_y);i++)
	{
	
	fwrite(game_data->board[i] ,game_data->size_x,1, savefile);
	
	}
	fclose (savefile);

	return 0;
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
							(*game_data).from_x=j;
							(*game_data).from_y=i;
							(*game_data).to_x=aux_j;
							(*game_data).to_y=aux_i;
							move_type = check_Move(game_data);

							if (  ((capt_aux=check_Captures(game_data, aux_i, aux_j)) == captures) )
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

int initial_Turn(void)
{
	return (rand()%2);
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
		        for ( k=0 ; k<360 ; k+=45 )
		        {
		      		aux_i=i;
					aux_j=j;
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
