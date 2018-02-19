#include "buffer.h" // buffer
#include <stdlib.h> /* required for rand() */
#include <pthread.h> // pthread mutex lock, pthread mutex unlock
#include <semaphore.h> // sem, sem_init, sem_wait, sem_post

pthread mutex_t mutex;
/* create the mutex lock */ 

//declare semaphore
sem_t empty, full;

//will be manipulated as a circular queue
/* the buffer */ 
buffer_item buffer[BUFFER_SIZE];

int main(int argc, char *argv[]) {

    sem_init(&sem, 0, 1);
    pthread mutex_init(&mutex,NULL);
    /* 1. Get command line arguments argv[1],argv[2],argv[3] */ 

    /* 2. Initialize buffer */ 
    /* 3. Create producer thread(s) */
    /* 4. Create consumer thread(s) */ 
    /* 5. Sleep */ /* 6. Exit */

 }

int insert_item(buffer_item item) { 
    /* insert item into buffer return 0 if successful,
     otherwise return -1 indicating an error condition */

    do {

        . . .

        /* produce an item in next produced */ . . .

        sem_wait(&empty); //
        pthread_mutex_lock(&mutex);
        . . .

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