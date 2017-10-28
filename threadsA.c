#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>

void *consumer(void*);
void *producer(void*);

int main (int argc, char **argv){
	int num_threads = atoi(argv[1]);
	pthread_t *threads;

	//allocate space for thread array
	threads =  (pthread_t*)calloc(num_threads,sizeof(pthread_t));

	int pcr;
	long i;
	for (i = 0; i < num_threads; i++){
		if (i % 2 == 0){
			if (pcr = pthread_create(&threads[i],NULL,&consumer,(void*)i)) {
				printf("%s\n", "Create failed.");
			}
		}
		else {
			if (pcr = pthread_create(&threads[i],NULL,&producer,(void*)i)) {
				printf("%s\n", "Create failed.");
			}
		}
	}
	for (i = 0; i < num_threads; i++){
		pthread_join(threads[i],NULL);
	}
	free(threads);
}

void *consumer(void *arg){
	int id = (int)arg;
	printf("PID: %d reporting in.\n", id);
	printf("%s\n", "I am a consumer.");
}

void *producer(void *arg){
	int id = (int)arg;
	printf("PID: %d reporting in.\n", id);
	printf("%s\n", "I am a producer.");
}