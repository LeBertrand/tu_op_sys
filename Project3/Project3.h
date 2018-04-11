#include "../csapp.c"
#include "spellchecker.h"
#include <sys/socket.h>

/*
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>

#ifndef CSAPP
#define CSAPP

#endif
*/

char *DEFAULT_DICTIONARY = "dictionary";
const char LONGEST_WORD = 30;
const int DEFAULT_PORT = 5000;
const int NUM_WORKERS = 5;
const int Q_LEN = 20;

#ifndef BOOLEAN_T
#define BOOLEAN_T
typedef enum { false = 0, true = 1 } boolean_t;
#endif


/*********************************
Fixed size circular buffer type
Dumb struct and methods. Will over-write itself.
Requires CV, mutex and adult supervision.
*********************************/
typedef struct {
    // Array of integers
    int *sd_arr;
    // Number of integers spaces allocated
    char sd_length;
    // Number of integers curently stored
    char sd_count;
    // Position of least recent integer insertion
    char sd_first;
    // Position of most recent integer insertion
    char sd_last;

    // Array of strings
    char **msg_arr;
    // Number of strings spaces allocated
    char msg_length;
    // Number of strings curently stored
    char msg_count;
    // Position of least recent string insertion
    char msg_first;
    // Position of most recent string insertion
    char msg_last;
} mx_circbuf_t;

/*
 *  Setup structure, zero out values and allocate space.
 *  Input:
    newbuf - Pointer to location of new structure.
    len - number of items to store in buffer.
 */
void mx_circbuf_init(mx_circbuf_t *newbuf, int len)
{
    newbuf->sd_arr = (int*) malloc(len * sizeof(int));
    newbuf->sd_length = len;
    newbuf->sd_count = 0;
    newbuf->sd_first = 0;
    newbuf->sd_last = 0;
    
    char log_q = 100;
    newbuf->msg_arr = (char**) malloc(len * sizeof(char*));
    newbuf->msg_length = log_q;
    newbuf->msg_count = 0;
    newbuf->msg_first = 0;
    newbuf->msg_last = 0;
    
}

/*
 *  Put new socket into sockets buffer. Increment insertion position and count.
 */
void mx_circbuf_acceptsd(mx_circbuf_t *buffer, int input)
{
    buffer->sd_arr[buffer->sd_last++] = input;
    buffer->sd_last = buffer->sd_last % buffer->sd_length;
    buffer->sd_count++;
}

/*
 *  Get socket from sockets buffer. Increment removal position and decrement count.
 *  Return: Socket descriptor removed from buffer.
 
 *  Data isn't removed in the sense that it's overwritten. Circular buffer records
    position to get next data from, and that position gets incremented.
 */
int mx_circbuf_offersd(mx_circbuf_t *buffer)
{
    int output = buffer->sd_arr[buffer->sd_first++];
    buffer->sd_first = buffer->sd_first % buffer->sd_length;
    buffer->sd_count--;
    return output;
}

/*
 *  Put new log message into msg buffer. Increment insertion position and count.
 */
void mx_circbuf_acceptmsg(mx_circbuf_t *buffer, char *input){
    buffer->msg_arr[buffer->msg_last++] = input;
    buffer->msg_last = buffer->msg_last % buffer->msg_length;
    buffer->msg_count++;
}

/*
 *  Get log msg from msg buffer. Increment removal position and decrement count.
 
 *  Message isn't removed in the sense that it's overwritten. Circular buffer records
    position to get next data from, and that position gets incremented.
 */
char *mx_circbuf_offermsg(mx_circbuf_t *buffer)
{
    char *output = buffer->msg_arr[buffer->msg_first++];
    buffer->msg_first = buffer->msg_first % buffer->msg_length;
    buffer->msg_count--;
    return output;
}

void mx_circbuf_destructor(mx_circbuf_t *DOA)
{
    char i;
    for(i = 0; i < DOA->msg_length; i++){
        free(DOA->msg_arr[i]);
    }
    free(DOA->msg_arr);
    free(DOA->sd_arr);
    free(DOA);
}

/*
void mx_circbuf_destructor(mx_circbufint_t *DOA)
{
    char i;

    free(DOA->content);
    free(DOA);
} */

/************************************************
Function headers for Networked Spell Checker.
************************************************/
/*
 * Safely insert a socket descriptor into a buffers-struct
    concurrently with other threads.

 *  Inputs
    sd - socketdescriptor for connected client.
 */
 void append_sockets(int sd);

/*
 *  Safely remove socket descriptor from buffers-struct concurrently
    with other threads.
 */
int take_socket();

/*
 *  Safely insert string into buffers-struct log buffer concurrently with other
    threads. Copies argument into heap allocated string.
 *  Inputs
    lookup - string containing word and response
 */
void append_lookups(char *lookup);

/*
 *  Safely remove string from buffers-struct log buffer concurrently with other
    threads. 
 */
char *take_lookup();

/*
 *  Try to consume from socket_requests
 *  For all connections taken from sockets buffer:
 *      Lookup word
 *      Send word with response appended
 *      Log word and response
 *  When client disconnects, startover.
 */
void *worker_routine(void *);

/*
 *  Get next lookup record from buffers-struct log buffer and print it to log file.
 */
void *logger_routine(void *);