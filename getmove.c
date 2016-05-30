int getmove(int upnext, char board[BOARD_SIZE_MAX_Y][BOARD_SIZE_MAX_X])
{
    int cuantosleyo=0, jugada=0;
	display_board(board);
	printf("Jugador %d escriba su próxima jugada\n",upnext);
    while(!jugada)
    {
        while((cuantosleyo=scanf("[%d,%d] [%d,%d]", &from_x, &from_y, &to_x, &to_y))!=4 || getchar()!='\n')
        {
            BORRA_BUFFER;
            display_board(board);
            printf("Error: Lectura de parámetros incorrectos. Intente nuevamente\nJugador %d escriba su próxima jugada\n", upnext);
        }
        jugada=check_move(from_x, from_y, to_x, to_y, board, upnext);
        if(!jugada)
        {
            display_board(board);
		    printf("Error: Jugada Imposible. Intente nuevamente\nJugador %d escriba su próxima jugada\n",upnext);
        }
    }
    return jugada;
}
