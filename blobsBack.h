#define BOARD_SIZE_MAX_Y 30
#define BOARD_SIZE_MAX_X 30

int check_move(const char *move);

void modify_board(const char *move, char board[][]);

int turn(int mode);