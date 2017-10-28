#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <stdint.h>
#include <unistd.h>

//declare functions
void *job1(void*);
void *job2(void*);

//global, shared variables
int A = 0;
int B = 0;

//create semaphore
sem_t lock;

int main (int argc, char **argv){
	int num_threads = atoi(argv[1]);
	pthread_t *threads;

	//allocate space for thread array
	threads =  (pthread_t*)calloc(num_threads,sizeof(pthread_t));

	//initialize semaphore
	if (sem_init(&lock,0,1) < 0){
		printf("%s\n", "Error initializing semaphore.");
		return 0;
	}

	//create the threads
	int pcr;
	long i;
	for (i = 0; i < num_threads; i++){
		if (i % 2 == 0){
			if (pcr = pthread_create(&threads[i],NULL,&job1,(void*)i)) {
				printf("%s\n", "Create failed.");
			}
		}
		else {
			if (pcr = pthread_create(&threads[i],NULL,&job2,(void*)i)) {
				printf("%s\n", "Create failed.");
			}
		}
	}
	for (i = 0; i < num_threads; i++){
		pthread_join(threads[i],NULL);
	}

	//free the threads
	free(threads);

	//destroy that semaphore
	sem_destroy(&lock);

	//print the output
	printf("A: %d\n", A);
	printf("B: %d\n", B);

	return 0;
}

void *job1(void *arg){
	int id = (uintptr_t)arg;
	printf("PID: %d reporting in. Executing job 1!\n", id);
	//doing the addition for job 1
	for (int i = 0; i < 5; i++){
		//do i need to do two sem_waits? or just one?
		sem_wait(&lock);
		A += 1;
		usleep(101);
		B += 2;
		usleep(101);
		sem_post(&lock);
	}
}

void *job2(void *arg){
	int id = (uintptr_t)arg;
	printf("PID: %d reporting in. Executing job 2!\n", id);
	//doing the addition for job 2
	for (int i = 0; i < 5; i++){
		sem_wait(&lock);
		B += 3;
		usleep(101);
		A += 1;
		usleep(101);
		sem_post(&lock);
	}
}