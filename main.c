#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#define THREADS 10
#define LEN 5


pthread_mutex_t mutex;  /* declare mutex */
pthread_cond_t cond_available;  /* declare a conditional variable for full queue state */
/* declare and initialize global variables */
char queue[10];
long unsigned buffer_available = 1023;

char** allFoundWords;
int currentIndex;
char* buffer[THREADS];
int buffer_size;


void* consumer(void* arg){
	char c = (int)arg / THREADS;
	int id = (int) arg % THREADS;
	int i, j;
	//Simple example of data consumption
	for(i = 0; i < 1000000;i++)
		j = i/23 + i%17;
	printf("Thread #%d: %c\n", id, c);
	//must be replaced by the actual code to search for words in the buffer.
	pthread_mutex_lock(&mutex);
	buffer_available |= ((long unsigned)1 << id);
	pthread_cond_signal(&cond_available);
	pthread_mutex_unlock(&mutex);
}
void main(int argc, char** argv){
	pthread_t threadID[THREADS];
	pthread_mutex_init(&mutex, NULL);/* initialize the mutex */
    pthread_cond_init(&cond_available, NULL);
	char c;
	int j, buffer_number;
	//? initializing threads
	for(j = 0; j < THREADS;j++)
		threadID[j] = NULL;
	long unsigned temp;
	while((c = getchar()) != EOF){
		pthread_mutex_lock(&mutex);
		while(!buffer_available)
			pthread_cond_wait(&cond_available, &mutex);
		temp = buffer_available;
		
		//? adding buffers?
		buffer_number = 0;
		while((temp & 1) == 0){
			temp = temp >> 1; //? shifting one bit to the right.
			buffer_number++;
		}
		buffer_available &= ~((long unsigned)1 << buffer_number);
		pthread_mutex_unlock(&mutex);

		//? Error handling
		if(pthread_create(threadID + buffer_number, NULL, consumer, (void *) (buffer_number + c * THREADS))){
			fprintf(stderr, "Error: Too many threads are created!\n");
			break;
		}
		//? End error handling
	}

	//? joining threads
	for(j = 0; j < THREADS;j++){
		if(!threadID[j])
			continue;
		pthread_join(threadID[j], NULL);
		fprintf(stderr, "Note: Thread %d joined!\n", j);
	}
}