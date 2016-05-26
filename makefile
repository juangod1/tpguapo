ALL:
	gcc -o Trabajo_Final_PI blobsFront.c blobsBack.c getnum.c
wall:
	gcc -o Trabajo_Final_PI blobsFront.c blobsBack.c getnum.c -Wall
clean:
	rm Trabajo_Final_PI