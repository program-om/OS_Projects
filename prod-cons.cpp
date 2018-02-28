// Note about running the file: use -lpthread for the semaphore
// clang++ prod-cons.cpp -lpthread -o prod-cons
// ./prod-cons n m l

#include "buffer.h" // buffer
#include <stdlib.h> /* required for rand() */
#include <pthread.h> // pthread mutex lock, pthread mutex unlock
#include <semaphore.h> // sem, sem_init, sem_wait, sem_post
#include <stdio.h>
#include <iostream>
#include <unistd.h> //sleep
#include <cstdlib> //rand()

pthread_mutex_t mutex_in, mutex_out;
/* create the mutex lock */ 

//declare semaphore
sem_t empty, full;

int in = 0, out = 0;

//will be manipulated as a circular queue
/* the buffer */ 
buffer_item buffer[BUFFER_SIZE];

//This fuction will take the item and will place it the buffer
int insert_item(buffer_item item);
// This function will increment the out pointer and It pass the item removed by referencd.
int remove_item(buffer_item &item);
//producer will produce an item in the buffer
void *producer(void *param);
//consumer will get and remove an item from the buffer
void *consumer(void *param);


int main(int argc, char *argv[]) {

    /* 1. Get command line arguments argv[1],argv[2],argv[3] */ 
    int sleepTime = atoi(argv[1]), //argv[0]: time for the main to sleep before terminating
        numProdTh = atoi(argv[2]), //argv[1]: number of producer threads
        numConsTh = atoi(argv[3]); //argv[2]: number of consumer threads

    if(argc != 4){
        printf("Error: There must be 3 command line arguments");
        return -1;
    }

    if (sleepTime < 0 || numProdTh < 0 || numConsTh < 0) {
        printf("Error: One of the argument is less than 0");
        return -1;
    }

    /* 2. Initialize buffer */ //with random number
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex_in,NULL);
    pthread_mutex_init(&mutex_out,NULL);

    /* 3. Create producer thread(s) */
    for(int i=0; i < numProdTh; i++){

        pthread_t tid; /* the thread identifier */ 
        pthread_attr_t attr; /* set of thread attributes */

        /* get the default attributes */ 
        pthread_attr_init(&attr);
        /* create the thread */ 
        pthread_create(&tid, &attr, producer, (void *)(long)i);

    }

    /* 4. Create consumer thread(s) */
    for(int i=0; i < numConsTh; i++){

        pthread_t tid; /* the thread identifier */ 
        pthread_attr_t attr; /* set of thread attributes */

        /* get the default attributes */ 
        pthread_attr_init(&attr);
        /* create the thread */
        //cast i to thread args
        pthread_create(&tid, &attr, consumer, (void *)(long)i);
    }

    /* 5. Sleep */
    sleep(sleepTime);
    /* 6. Exit */
    return 0;

 }


int insert_item(buffer_item item) { 

	pthread_mutex_lock(&mutex_in);
    //critical section
    buffer[in] = item;
    in = (in+1) % BUFFER_SIZE;
    pthread_mutex_unlock(&mutex_in);
        
    return 1;
}

int remove_item(buffer_item &item) { 

    pthread_mutex_lock(&mutex_out);
    //critical section
    item = buffer[out];
    out = (out+1) % BUFFER_SIZE;
    pthread_mutex_unlock(&mutex_out);

    return 1;
}

void *producer(void *param) { 
    
    buffer_item item;
    int producerNum = (int)(long)param;

    while (true) {
        /* sleep for a random period of time */
        int time = rand()%3;
        sleep(time);
        item = rand()%10000; // generate a random number
        sem_wait(&empty); //wait if the buffer is full
        insert_item(item);

        printf("producer %d produced %d\n", producerNum, item);
        //decrement the number of the empty slots by
	    sem_post(&full);
    }
    pthread_exit(0);
}

void *consumer(void *param) { 

    buffer_item item;
    int consumerNum = (int)(long)param;

    while (true) {

        /* sleep for a random period of time */ 
        int time = rand()%3;
        sleep(time);
        sem_wait(&full); //decrement full items
        remove_item(item);
            
        printf("consumer %d consumed %d\n", consumerNum, item);
	    sem_post(&empty); //increment empty items
    }
    pthread_exit(0);
}