#include <stdio.h>
#include <stdlib.h>

int main(void)
	{
		int i;
		char vec[15]="save", fvect[15]={};
		for(i=0; i<15; i++)
			printf("%d",vec[i]);
		fgets(fvect, 15, stdin);
			for(i=0; i<15; i++)
		printf("%d",fvect[i]);
	}
