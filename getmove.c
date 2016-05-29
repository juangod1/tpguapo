void getmove(int upnext, char board[BOARD_SIZE_MAX_Y][BOARD_SIZE_MAX_X])
{
	int jugadacorrecta=0;
	display_board(board);
	printf("Jugador %d escriba su próxima jugada\n",upnext);
	while(!jugadacorrecta)
	{
		from_x=-1, from_y=-1, to_x=-1, to_y=-1
		scanf("[%d,%d][%d,%d]", &from_x, &from_y, &to_x, &to_y);
		if(getchar()!='\n'|| from_x==-1 || from_y==-1 || to_x==-1 || to_y==-1)
		{
			BORRA_BUFFER;
			display_board(board);
			printf("Error: Lectura de parámetros incorrectos. Intente nuevamente\n");
		}
		else
		{
			jugadacorrecta=check_move(from_x, from_y, to_x, to_y, board, upnext))
			if(jugadacorrecta)
				display_board(board);
				printf("Error: Jugada Imposible. Intente nuevamente\n");
		}
	}
}
