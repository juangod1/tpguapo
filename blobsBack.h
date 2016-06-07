#define BOARD_SIZE_MAX_Y 30
#define BOARD_SIZE_MAX_X 30

extern int size_x, size_y, from_x, from_y, to_x, to_y;

int check_Move(int from_x, int from_y, int to_x, int to_y, char board[30][30], int upnext);

int initial_Turn(int mode);

int end_Game(char board[BOARD_SIZE_MAX_Y][BOARD_SIZE_MAX_X], int upnext);

void modify_Adjacent_Blocks(char board[30][30]);

void modify_Board(char board[BOARD_SIZE_MAX_Y][BOARD_SIZE_MAX_X], int move_type, int upnext);

int get_Move_AI(char board[][BOARD_SIZE_MAX_X]);

int open_file(char *filename);

int save_file(char *filename, int mode,int upnext,int size_y, int size_x,char board[30][30]);