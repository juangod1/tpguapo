#define BOARD_SIZE_MAX_Y 30
#define BOARD_SIZE_MAX_X 30

typedef struct
{
	int size_x, size_y, from_x, from_y, to_x, to_y;
	int upnext, mode;
	char board[BOARD_SIZE_MAX_X][BOARD_SIZE_MAX_Y];
} game_data_type;
