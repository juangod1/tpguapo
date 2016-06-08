#define BOARD_SIZE_MAX_Y 30
#define BOARD_SIZE_MAX_X 30

typedef struct
{
	int size_x, size_y, from_x, from_y, to_x, to_y;
	int upnext, mode;
	char board[BOARD_SIZE_MAX_X][BOARD_SIZE_MAX_Y];
} game_data_type;

int check_Move(game_data_type *game_data);

int initial_Turn(int mode);

int end_Game(game_data_type *game_data);

void modify_Adjacent_Blocks(game_data_type *game_data);

void modify_Board(game_data_type *game_data, int move_type);

int get_Move_AI(game_data_type *game_data);

int open_File(char *filename, int mode,int upnext,int size_y, int size_x,char board[30][30]);

int save_File(char *filename, int mode,int upnext,int size_y, int size_x,char board[30][30]);

