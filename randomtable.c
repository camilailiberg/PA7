#include<stdlib.h>
#include<stdio.h>
#include<string.h>
void main(int argc, char** argv){
	if(argc != 3)
		fprintf(stderr, "Error: The program expect two command-line arguments which are the number of rows and columns of randomly generated table respectively.\n");
	int n = atoi(argv[1]);
	int m = atoi(argv[2]);
	long k = n * m;
	srand(time(NULL));
	long l = 0;
	while(l++ < k){
		putchar('a'+rand()%26);
		if(l%m == 0)
			putchar('\n');
	}
	char* x = (char*) malloc(64000000);
}