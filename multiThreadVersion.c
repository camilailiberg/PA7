#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char** argv){
	if (argc < 7)
		fprintf(stderr, "Error: too few CLAs.");
	long row = 0;
	int column = 0, linesize = 0, size = 0, i = 0, fd;
	char* filename = NULL;
	char** buffer;
	while(++i < argc){
		if(argv[i][0] != '-'){
			fprintf(stderr, "Error: invalid CLA.\n");
			break;
		}
		if(!strcmp(argv[i], "-line"))
			linesize = atoi(argv[++i]);
		else if(!strcmp(argv[i], "-size"))
			size = atoi(argv[++i]);
		else if(!strcmp(argv[i], "-row"))
			row = (long)atoi(argv[++i]);
		else if(!strcmp(argv[i], "-column"))
			column = atoi(argv[++i]);
		else if(!strcmp(argv[i], "-file")){
			filename = (char*)malloc(1 + strlen(argv[++i]));
			strcpy(filename, argv[i]);
		}else{
			fprintf(stderr, "Error: invalid CLA.");
			break;
		}
	}
	if(filename == NULL || size * linesize == 0){
		fprintf(stderr, "Error: invalid CLA.");
		exit(1);
	}
	long start = row * (linesize+1) + column;
	printf("start index is %d\n", start);
	if((fd = open(filename, O_RDONLY, 0)) == -1){
		fprintf(stderr, "Error: can't open the file %s.", filename);
		exit(1);
	}
	buffer = (char**)malloc(size * sizeof(char*));
	for(i = 0;i < size;i++)
		buffer[i] = (char*)malloc(size);
	lseek(fd,start,SEEK_SET);
	for(i = 0; i < size;i++){
		read(fd, buffer[i], size);
		lseek(fd, linesize-size+1, SEEK_CUR);
	}
	for(i = 0;i < size;i++){
		for(int j = 0; j < size;j++)
			printf("%c",buffer[i][j]);
		printf("\n");
	}
}