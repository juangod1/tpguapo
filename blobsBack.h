#define BOARD_SIZE_MAX_Y 30
#define BOARD_SIZE_MAX_X 30

extern int size_x, size_y, from_x, from_y, to_x, to_y;

int check_move(int from_x, int from_y, int to_x, int to_y, char board[30][30], int upnext);

int init_turn(int mode);

int endgame(char board[BOARD_SIZE_MAX_Y][BOARD_SIZE_MAX_X], int upnext);

void modifyAdjacentBlocks(char board[30][30]);
