#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <stdint.h>

#define BUF_SIZE 3
#define LOOP_COUNT 20

//consumer/producer function
void *consumer(void*);
void *producer(void*);
void append(int);
int take();

//create semaphore
sem_t s;
sem_t n;
sem_t e;

int *buffer;
int buffer_p = 0;

int main (int argc, char **argv){
	int num_threads = atoi(argv[1]);
	pthread_t *threads;

	//allocate space for thread array
	threads =  (pthread_t*)calloc(num_threads,sizeof(pthread_t));
	//allocate space for buffer
	buffer = (int*)calloc(BUF_SIZE,sizeof(int));
	
	//initialize semaphores
	if (sem_init(&s,0,1) < 0){
		printf("%s\n", "Error initializing semaphore.");
		return 0;
	}
	if (sem_init(&n,0,0) < 0){
		printf("%s\n", "Error initializing semaphore.");
		return 0;
	}
	if (sem_init(&e,0,BUF_SIZE) < 0){
		printf("%s\n", "Error initializing semaphore.");
		return 0;
	}
	
	//create the threads
	int pcr;
	long i;
	for (i = 0; i < num_threads; i++){
		if (i % 2 == 0){
			if (pcr = pthread_create(&threads[i],NULL,&producer,(void*)i)) {
				printf("%s\n", "Create failed.");
			}
		}
		else {
			if (pcr = pthread_create(&threads[i],NULL,&consumer,(void*)i)) {
				printf("%s\n", "Create failed.");
			}
		}
	}
	for (i = 0; i < num_threads; i++){
		pthread_join(threads[i],NULL);
	}
	
	//free threads
	free(threads);
	
	//destroy that semaphore
	sem_destroy(&s);
	sem_destroy(&n);
	sem_destroy(&e);
	
	return 0;
}

void *consumer(void *arg){
	int id = (uintptr_t)arg;
	for (int i = 0; i < LOOP_COUNT; i++){
		sem_wait(&n);
		sem_wait(&s);
		int t = take();
		printf("PID %d reporting in. ", id);
		printf("I consumed: %d\n", t);
		sem_post(&s);
		sem_post(&e);
	}

}

void *producer(void *arg){
	int id = (uintptr_t)arg;
	for (int i = 0; i < LOOP_COUNT; i++){
		//produce
		int produce = rand() % 20;
		printf("PID %d reporting in. ", id);
		printf("I produced: %d\n", produce);
		sem_wait(&e);
		sem_wait(&s);
		append(produce);
		sem_post(&s);
		sem_post(&n);
	}
}

void append(int a){
	if (buffer_p < BUF_SIZE){
		buffer[buffer_p++] = a;
	}
}

int take(){
	int t = buffer[--buffer_p];
	return t;
}
