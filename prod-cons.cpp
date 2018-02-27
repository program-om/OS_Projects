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

int insert_item(buffer_item item);
int remove_item(buffer_item *item);
void *producer(void *param);
void *consumer(void *param);


int main(int argc, char *argv[]) {

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

    sem_init(&empty, 0, BUFFER_SIZE);
    
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex_in,NULL);
    pthread_mutex_init(&mutex_out,NULL);

    
    /* 1. Get command line arguments argv[1],argv[2],argv[3] */ 

    /* 2. Initialize buffer */ //with random number
    
//     for(int i=0; i < BUFFER_SIZE; i++){
//         int randomNum = rand();
//         buffer[i] = randomNum;
//     }

    /* 3. Create producer thread(s) */
    for(int i=0; i < numProdTh; i++){

        pthread_t tid; /* the thread identifier */ 
        pthread_attr_t attr; /* set of thread attributes */

        /* get the default attributes */ 
        pthread_attr_init(&attr);
        /* create the thread */ 
        pthread_create(&tid, &attr, producer, (void *)(long)i);

    }

	sleep(3);
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
    /* 6. Exit */
    sleep(sleepTime);
    return 0;

 }

int insert_item(buffer_item item) { 
    /* insert item into buffer return 0 if successful,
     otherwise return -1 indicating an error condition */

        /* produce an item in next produced */
        bool successful;
        
	pthread_mutex_lock(&mutex_in);
        buffer[in] = item;
	//std::cout << "nope" << std::endl;
        if(buffer[in] == item){//successful
            successful = true;
	    //std::cout << buffer[in] << std::endl;
        } else{//error
            successful = false;
        }
	//std::cout << "nope" << std::endl;
        in = (in+1) % BUFFER_SIZE;
        pthread_mutex_unlock(&mutex_in);
        /* add next produced to the buffer */
	//std::cout << "nope" << std::endl;
        if(successful)
            return 1;
        else
            return 0;
    
}

int remove_item(buffer_item *item) { 
    /* remove an object from buffer placing it in item return 0 if successful,
     otherwise return -1 indicating an error condition */
     bool successful;

        /* produce an item in next produced */

        pthread_mutex_lock(&mutex_out);
        item = &buffer[out];
        buffer[out] = 0;

        if(buffer[out] == 0){
            successful = true;
        } else{
            successful = false;
        }

        out = (out+1) % BUFFER_SIZE;
        pthread_mutex_unlock(&mutex_out);
        /* add next produced to the buffer */
        if(successful) 
            return 1;
        else 
            return 0;
        
     
}

void *producer(void *param) { 
    
    buffer_item item;
    int producerNum = (int)(long)param;

    while (true) {
        /* sleep for a random period of time */ 
        sleep(1);
        /* generate a random number */ 
        item = rand()%10000;
        
        sem_wait(&empty); //decrement empty items
	    
        if (!insert_item(item)){
            printf("report error condition");
        } else{
            printf("producer %d produced %d\n", producerNum, item);
        }
	sem_post(&full); //increment full items
    }
    pthread_exit(0);
}

void *consumer(void *param) { 

    buffer_item item;
    int consumerNum = (int)(long)param;

    while (true) {

        /* sleep for a random period of time */ 
        sleep(1);
//std::cout << "nope 1" << std::endl;
	
        sem_wait(&full); //decrement full items
     
	std::cout << "nope 2" << std::endl;
        if (!remove_item(&item)){
            printf("report error condition");
        } else{
	    //std::cout << "nope." << std::endl;
            printf("consumer %d consumed %d\n", consumerNum, item);
        }
	sem_post(&empty); //increment empty items
	//std::cout << "no seg" << std::endl;
    }
    pthread_exit(0);
}


//using mutex:
/* acquire the mutex lock */ 
//pthread_mutex_lock(&mutex); 
/* critical section */
/* release the mutex lock */
//pthread_mutex_unlock(&mutex);



//using semaphore:
/* Create the semaphore and initialize it to 1 */ 

/* acquire the semaphore */ 
//sem_wait(&sem); 
/* critical section */ 
/* release the semaphore */ 
//sem_post(&sem);

/*'
 create n th prod
 create x th cons
 sleep -> while sleeping cons and prod will infinitely create new elements and remove elements
 terminate
*/
