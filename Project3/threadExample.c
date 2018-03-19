#include <pthread.h>
#include <stdio.h>
#define WORKER_COUNT 20

void* threadFunction(void*);

int main(int argc, char** argv)
{
    //Create an array of threads.
    pthread_t threadPool[WORKER_COUNT];
    int threadIDs[WORKER_COUNT];
    int i;
    printf("Launching threads.\n");
    for(i = 0; i < WORKER_COUNT; i++){
        threadIDs[i] = i;
        //Start running the threads.
        pthread_create(&threadPool[i], NULL, &threadFunction, &threadIDs[i]);
    }
    printf("All threads launched, waiting for them to quit.\n");
    for(i = 0; i < WORKER_COUNT; i++){
        //Wait for all threads to finish executing.
        pthread_join(threadPool[i], NULL);
    }
    printf("All threads completed.\n");
    return 0;
}

void* threadFunction(void* id){
    printf("Thread %d: Hello\n", *((int*) id));
    printf("Thread %d: Quitting\n", *((int*) id));
}