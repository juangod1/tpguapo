ALL:
	gcc -o Trabajo_Final_PI blobsFront.c blobsBack.c -lm
wall:
	gcc -o Trabajo_Final_PI blobsFront.c blobsBack.c -Wall -lm
clean:
	rm Trabajo_Final_PI
