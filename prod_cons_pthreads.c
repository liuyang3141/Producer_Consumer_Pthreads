#include <stdlib.h> // for rand() //
#include <pthread.h>	/* for pthread functions */
#include <stdio.h> // for input/output
#include <semaphore.h> // for synchronization locks
#include <unistd.h> // for sleep

// buffer for producer and consumer to share
#define BUFFER_SIZE 5
typedef int buffer_item;
buffer_item buffer[BUFFER_SIZE];

// indices for buffer
int in = 0;
int out = 0;
int count = 0;

int produced = 0; // keeps track of number of items produced
int consumed = 0; // keeps track of number of items consumed

// semaphore locks for synchronization
sem_t mutex;
sem_t empty;
sem_t full;

void *producer(void *param); // producer thread runs this function
void *consumer(void *param); // consumer thread runs this function

int insert_item(buffer_item item); // producer inserts item to buffer with this function
int remove_item(buffer_item *item); // consumer removes item from buffer with this function

int main(int argc, char *argv[])
{
	char *p; // for converting string from argv[] to long int
	long convert; // for storing converted long int
	int runTime; // for storing length of time program should run
	int numProdThreads; // for storing number of producer threads
	int numConsThreads; // for storing number of consumer threads
	int i; // loop index

	// converting argv[] to int
	convert = strtol(argv[1], &p, 10);
	runTime = convert;

	convert = strtol(argv[2], &p, 10);
	numProdThreads = convert;

	convert = strtol(argv[3], &p, 10);
	numConsThreads = convert;

	// initializing semaphore locks
	sem_init(&mutex, 0, 1);
	sem_init(&empty, 0, BUFFER_SIZE);
	sem_init(&full, 0, 0);

	pthread_t producerTid[numProdThreads]; 	/* the producer thread identifier */
	pthread_t consumerTid[numConsThreads]; 	/* the consumer thread identifier */
	pthread_attr_t attr; /* set of attributes for the thread */

	/* get the default attributes */
	pthread_attr_init(&attr);

	// initializing buffer
	for (i = 0; i < BUFFER_SIZE; ++i) {
		buffer[i] = -1;
	}

	srand(time(NULL)); // seeding rand

	printf("Main thread beginning\n");

	/* create the producer threads */
	for (i = 0; i < numProdThreads; i++) { 
		/* Note that we are passing the thread id by reference */
		pthread_create(&producerTid[i], &attr, producer, (void *) &producerTid[i]); 
		printf("Creating producer thread with id %lu\n", producerTid[i]);
    }

	/* create the consumer threads */
	for (i = 0; i < numConsThreads; i++) { 
		/* Note that we are passing the thread id by reference */
		pthread_create(&consumerTid[i], &attr, consumer, (void *) &consumerTid[i]); 
		printf("Creating consumer thread with id %lu\n", consumerTid[i]);
    }

	printf("Main thread sleeping for %d seconds\n", runTime);
	// putting main thread to sleep
	sleep(runTime);

	// output total items produced and consumed for convenience
	printf("\nTotal items produced = %d\nTotal items consumed = %d\n", produced, consumed);

	printf("Main thread exiting\n");
	exit(0);
}

void *producer(void *param) {
	pthread_t threadID = *((pthread_t *) param); // getting thread ID
	

	while(1) {
		int sleepTime = (rand() % 4) + 1; // generate random sleep time
		printf("Producer thread %lu sleeping for %d seconds\n", threadID, sleepTime);
		sleep(sleepTime);

		buffer_item	item; // for storing item to be created
		item = (rand() % 50) + 1; // producing an item

		while(count == BUFFER_SIZE) // don't produce anything if buffer is full
			;
		
		if (insert_item(item) < 0) { // attempt to add item to buffer
			printf("Produer error\n");
		}
		else {
			printf("Producer thread %lu inserted value %d\n", threadID, item);
		}
	}
}

void *consumer(void *param) {
	pthread_t threadID = *((pthread_t *) param); // getting thread ID
	buffer_item item; // for storing item to be consumed

	while(1) {
		int sleepTime = (rand() % 4) + 1; // generate random sleep time
		printf("Consumer thread %lu sleeping for %d seconds\n", threadID, sleepTime);
		sleep(sleepTime);

		while(count == 0) // don't consume anything if buffer is empty
			;
		
		if (remove_item(&item) < 0) { // attempt to consume an item from buffer
			printf("Consumer error\n");
		}
		else {
			printf("Consumer thread %lu removed value %d\n", threadID, item);
		}
	}
}

int insert_item(buffer_item item) {
	if (in < 0 || in >= BUFFER_SIZE || count < 0 || count > BUFFER_SIZE) // error checking
		return 0;
	
	int x; // for loop index

	sem_wait(&empty); // decrement empty and lock
	sem_wait(&mutex); // decrement mutex and lock

	buffer[in] = item; // add item to buffer

	printf("Insert_item inserted item %d at position %d\n", item, in);

	in = (in + 1) % BUFFER_SIZE; // increment index

	for (x = 0; x < BUFFER_SIZE; ++x) {
		if(buffer[x] == -1){
			printf("[empty]");
		}
		else {
			printf("[%d]", buffer[x]);
		}
	}
	printf(" in = %d, out = %d\n", in, out);

	
	++count; // increment amount of items in buffer
	++produced; // keep track of how many items were produced

	sem_post(&mutex); // increment mutex and release lock
	sem_post(&full); // increment full and release lock

	return 1;
}

int remove_item(buffer_item *item) {
	if(out < 0 || out >= BUFFER_SIZE || count < 0 || count > BUFFER_SIZE) // error checking
		return 0;

	int x; // for loop index

	sem_wait(&full); // decrement full and lock
	sem_wait(&mutex); // decrement mutex and lock

	*item = buffer[out]; // assign item to be consumed
	buffer[out] = -1;

	printf("Remove_item removed item %d at position %d\n", *item, out);

	out = (out + 1) % BUFFER_SIZE; // increment index

	for (x = 0; x < BUFFER_SIZE; ++x) {
		if(buffer[x] == -1){
			printf("[empty]");
		}
		else {
			printf("[%d]", buffer[x]);
		}
	}
	printf(" in = %d, out = %d\n", in, out);

	
	--count; // decrement amount of items in buffer
	++consumed; // keep track of how items were consumed

	sem_post(&mutex); // increment mutex and release lock
	sem_post(&empty); // decrement empty and release lock
	
	return 1;
}
