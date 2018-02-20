#include "buffer.h" // buffer
#include <stdlib.h> /* required for rand() */
#include <pthread.h> // pthread mutex lock, pthread mutex unlock
#include <semaphore.h> // sem, sem_init, sem_wait, sem_post

pthread_mutex_t mutex;
/* create the mutex lock */ 

//declare semaphore
sem_t empty, full;

//will be manipulated as a circular queue
/* the buffer */ 
buffer_item buffer[BUFFER_SIZE];

int main(int argc, char *argv[]) {

    int sleepTime = atoi(argv[0]), //argv[0]: time for the main to sleep before terminating
        numProdTh = atoi(argv[1]), //argv[1]: number of producer threads
        numConsTh = atoi(argv[2]); //argv[2]: number of consumer threads

    if(argc != 3){
        fprintf("Error: There must be 3 command line arguments");
        return -1;
    }

    if (sleepTime < 0 || numProdTh < 0 || numConsTh < 0) {
        fprintf("Error: One of the argument is less than 0");
        return -1;
    }

    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex,NULL);

    
    /* 1. Get command line arguments argv[1],argv[2],argv[3] */ 

    /* 2. Initialize buffer */ //with random number
    for(int i=0; i < BUFFER_SIZE; i++){
        int randomNum = rand();
        buffer[i] = randomNum;
    }

    /* 3. Create producer thread(s) */
    for(int i=0; i < numProdTh; i++){

        pthread_t tid; /* the thread identifier */ 
        pthread_attr_t attr; /* set of thread attributes */

        /* get the default attributes */ 
        pthread attr init(&attr);
        /* create the thread */ 
        pthread create(&tid, &attr, producer, argv[1]);

    }
    /* 4. Create consumer thread(s) */
    for(int i=0; i < numConsTh; i++){

        pthread_t tid; /* the thread identifier */ 
        pthread_attr_t attr; /* set of thread attributes */

        /* get the default attributes */ 
        pthread attr init(&attr);
        /* create the thread */
        //cast i to thread args
        pthread create(&tid, &attr, consumer, argv[1]);
    }
    /* 5. Sleep */
    /* 6. Exit */

    sleep(numConsTh);
    return 0;

 }

int insert_item(buffer_item item) { 
    /* insert item into buffer return 0 if successful,
     otherwise return -1 indicating an error condition */

    do {


        /* produce an item in next produced */ . . .

        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        if(){

        } else{

        }
        /* add next produced to the buffer */ . . .

        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    } while (true);
    
}

int remove_item(buffer_item *item) { 
    /* remove an object from buffer placing it in item return 0 if successful,
     otherwise return -1 indicating an error condition */
     
}

void *producer(void *param) { 
    buffer_item item;

    while (true) {

        /* sleep for a random period of time */ 
        sleep(...); 
        /* generate a random number */ 
        item = rand();
        if (insert_item(item)){
            fprintf("report error condition");
        } else{
            printf("producer produced %d\n", item);
        }
    }
    pthread_exit(0);
}

void *consumer(void *param) { 

    buffer_item item;

    while (true) {

        /* sleep for a random period of time */ 
        sleep(...);
        if (remove_item(&item)){
            fprintf("report error condition");
        } else{
            printf("consumer consumed %d\n",item);
        }
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