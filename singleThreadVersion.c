#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int dictionary_size;
char* dict[100000];
int binsearch(char* dictionaryWords[],int listSize,char* keyword){
	int mid, low = 0, high = listSize - 1;
	while (high >= low) {
		mid = (high + low) / 2;
		if (strcmp(dictionaryWords[mid],keyword) < 0)
			low = mid + 1;
		else if (strcmp(dictionaryWords[mid],keyword)>0)
			high = mid - 1;
		else
			return mid;
	}
	return -1; //not found
}
void dictionary_initialize(char* dict_filename){
	FILE* f = fopen(dict_filename, "r");
	char temp[1000], line[1000];
	dictionary_size = 0;
	while(fgets(line, 1000, f)){
		sscanf(line, "%s\n", temp);
		if(strlen(temp) == 0)
			continue;
		dict[dictionary_size] = (char*) malloc(sizeof(temp)+1);
		strcpy(dict[dictionary_size++], temp);
	}
	fclose(f);
}
int main(int argc, char** argv){
	if (argc < 15){
		fprintf(stderr, "Error: too few CLAs.\n");
		exit(1);
	}	
	long row = 0;
	int column = 0, linesize = 0, size = 0, i = 0, fd, len, j, k;
	char* filename = NULL;
	char* dict_filename;
	char* given_word;
	char** buffer;
	while(++i < argc){
		if(argv[i][0] != '-'){
			fprintf(stderr, "Error: invalid CLA.\n");
			exit(1);
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
		}else if(!strcmp(argv[i], "-dict")){
			dict_filename = (char*)malloc(1 + strlen(argv[++i]));
			strcpy(dict_filename, argv[i]);
		}else if(!strcmp(argv[i], "-len"))
			len = atoi(argv[++i]);
		else{
			fprintf(stderr, "Error: invalid CLA.\n");
			exit(1);
		}
	}
	if(filename == NULL || size * linesize == 0){
		fprintf(stderr, "Error: invalid CLA.");
		exit(1);
	}
	long start = row * (linesize+1) + column;
	if((fd = open(filename, O_RDONLY, 0)) == -1){
		fprintf(stderr, "Error: can't open the file %s.", filename);
		exit(1);
	}
	buffer = (char**)malloc(size * sizeof(char*)); //?allocating space for the buffer
	for(i = 0;i < size;i++)
		buffer[i] = (char*)malloc(size); //? allocating space for every element IN the buffer
	lseek(fd,start,SEEK_SET);
	for(i = 0; i < size;i++){
		read(fd, buffer[i], size);
		lseek(fd, linesize-size+1, SEEK_CUR);
	}
	dictionary_initialize(dict_filename);
	given_word = (char*)malloc(len+1);
	given_word[len] = '\0';
	//Horizontal/vertical search
	for(i = 0; i < size;i++)
		for(j = 0; j <= size - len;j++){
			//horizontal match...
			for(k = 0; k < len;k++)
				given_word[k] = buffer[i][k+j];
			if(binsearch(dict, dictionary_size, given_word) + 1)//if search is successful!
				printf("Horizontally Matched Word: %s\n", given_word);
			//vertical match...
			for(k = 0; k < len;k++)
				given_word[k] = buffer[k+j][i];
			if(binsearch(dict, dictionary_size, given_word) + 1)//if search is successful!
				printf("Vertically Matched Word: %s\n", given_word);
		}
	//Diagonal search
	for(i = 0; i <= size-len;i++)
		for(j = 0; j <=size-len;j++){
			//left to right...
			for(k = 0; k < len;k++)
				given_word[k] = buffer[i+k][j+k];
			if(binsearch(dict, dictionary_size, given_word) + 1)//if search is successful!
				printf("Diagonally (LR) Matched Word: %s\n", given_word);

			//right to left...
			for(k = 0; k < len;k++)
				given_word[k] = buffer[i+k][j+len-1-k];
			if(binsearch(dict, dictionary_size, given_word) + 1)//if search is successful!
				printf("Diagonally (RL) Matched Word: %s\n", given_word);



		}
}