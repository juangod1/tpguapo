#ifndef blobsBack_h
#define blobsBack_h

#define BOARD_SIZE_MAX_Y 30
#define BOARD_SIZE_MAX_X 30

typedef struct
{
	int size_x, size_y, from_x, from_y, to_x, to_y;
	int upnext, mode, blobsA, blobsZ;
	char board[BOARD_SIZE_MAX_X][BOARD_SIZE_MAX_Y];
} game_data_type;

int check_Move(game_data_type *game_data);
int initial_Turn(void);
void modify_Board(game_data_type *game_data, int move_type);
int get_Move_AI(game_data_type *game_data);
int save_File(char *filename, game_data_type * game_data);
int open_File(char *filename, game_data_type *game_data);
int end_Game(game_data_type *game_data);
#endif
