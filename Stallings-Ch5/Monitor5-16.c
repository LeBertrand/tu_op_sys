/*
 *  Functions from Stallings pg 230, Example 5.16.
 *  Modified by Shmuel Jacobs to use pthread library.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>

const int LENGTH = 10;

/*
 *  Variable names assume FIFO. First is first taken, most recent always
 *  becomes last. Count is number of items currently stored.
 */
int first = 0, last = 0, count = 0;
char *buf;
pthread_mutex_t mutex;
pthread_cond_t notfull, notempty;

void append(char ch);
void take(char ch);
void producer();
void consumer();


int main(int argc, char *argv[])
{
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&notfull, NULL);
    pthread_cond_init(&notempty, NULL);

    buf = (char*) malloc(LENGTH);
    memset(buf, '\0', LENGTH);

    return 0;
}
void append(char ch)
{
    if(count == LENGTH){
        pthread_cond_wait(&notfull, &mutex);
    }
    buf[last] = '*';
    last = ++last % LENGTH;
    count++;
    pthread_cond_signal(&notempty);
}

void take(char s)
{
    if(count == 0){ // Buffer is empty. Block until it isn't.
        pthread_cond_wait(&notempty, &mutex);
    }
    char c = buf[first];
    first = ++first % LENGTH;
    pthread_cond_signal(&notfull);
}
