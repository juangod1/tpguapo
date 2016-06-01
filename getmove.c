int getmove(upnext, board)
{
    char respuesta[15];
    int tipoinput=0, cantleidos, fromx,fromy,tox,toy;
    printf("Jugador %d escriba su proxima jugada\n", upnext);
    fgets(respuesta, 15, stdin);
    while(!tipoinput)
    {
        fgets(respuesta, 15, stdin);
        cantleidos=0;
        if(strcmp("save",respuesta)==0)
            tipoinput=3;
        if(strcmp("save",exit)==0)
            tipoinput=4;
        if(cantleidos !=4 && !tipoinput)
        {
            if(cantleidos=sscanf(respuesta, "(%d,%d)(%d,%d)", &fromx, &fromy, &tox, &toy)!=4)
            {
                BORRA_BUFFER;
                display_board(board);
                printf("Error de lectura de parametros, intente nuevamente.\nJugador %d escriba su proxima jugada\n", upnext)
                fgets(respuesta, 15, stdin);
            }
        }
        if(getchar()=='\n' && !tipoinput)
        {
            tipoinput=checkmove(from_x, from_y, to_x, to_y);
            display_board(board);

            if(!tipoinput)
            {
                display_board(board);
                printf("Error: Jugada Imposible. Intente Nuevamente\nJugador %d escriba su proxima jugada\n", upnext)
            }
        }
        
        
    }
    return tipoinput;
}
