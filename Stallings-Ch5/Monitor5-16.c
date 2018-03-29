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

struct {
    char ch;
} wrapper;

void append(char ch);
void take(char ch);
void *producer(void *);
void *consumer(void *);

char charcounter;
FILE *abcout;

int main(int argc, char *argv[])
{
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&notfull, NULL);
    pthread_cond_init(&notempty, NULL);
    charcounter = 0;
    buf = (char*) malloc(LENGTH);
    memset(buf, '\0', LENGTH);
    abcout = fopen("abcout.txt", "w");

    pthread_t threadp, threadc;
    pthread_create(&threadp, NULL, &producer, NULL);
    pthread_create(&threadc, NULL, &consumer, NULL);

    pthread_join(threadp, NULL);
    pthread_join(threadc, NULL);

    return 0;
}
void append(char ch)
{
    if(count == LENGTH){
        puts("Producer waiting for buffer not full");
        pthread_cond_wait(&notfull, &mutex);
    }
    buf[last] = ch;
    printf("Just put %c in the buffer.\n", ch);
    last = ++last % LENGTH;
    count++;
    puts("Producer signals buffer not empty");
    pthread_cond_signal(&notempty);
}

void take(char s)
{
    if(count == 0){ // Buffer is empty. Block until it isn't.
        pthread_cond_wait(&notempty, &mutex);
        puts("Consumer got signal buffer not empty");
    }
    char c = buf[first];
    printf("Just got %c from buffer.\n", c);
    wrapper.ch = c;

    first = ++first % LENGTH;
    pthread_cond_signal(&notfull);
}

void *producer(void *noarg)
{
    char x;
    unsigned int i;
    for(i = 0; i < 0xffffffff; i++){
        //x = 'A' + charcounter++;
        x = getchar();
        if(x == 'q') break;
        append(x);
    }
}

void *consumer(void *noarg){
    char x;
    char i;
    for(i = 0; i < 26; i++){
        take(x);
        // puts("Got another letter from buffer.");
        // printf("%c\n", wrapper.ch);

        fprintf(abcout, "%c", x);
    }
}
