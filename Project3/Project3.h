#include "../csapp.c"

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

const char *DEFAULT_DICTIONARY = "dictionary";
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


}

void mx_circbuf_acceptsd(mx_circbuf_t *buffer, int input)
{
    buffer->sd_arr[buffer->sd_last++] = input;
    buffer->sd_last = buffer->sd_last % buffer->sd_length;
    buffer->sd_count++;
}

int mx_circbuf_offersd(mx_circbuf_t *buffer)
{
    int output = buffer->sd_arr[buffer->sd_first++];
    buffer->sd_first = buffer->sd_first % buffer->sd_length;
    buffer->sd_count--;
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
 * Safely insert a socket descriptor into a buffers struct
    concurrently with other threads.

 *  Procedure:
    Lock socket_requests buffer.
    Wait for buffer not full.
    Put socket descriptor in next spot in buffer.
    Signal buffer not empty.
    Unlock buffer.

 *  Inputs
    sd - socketdescriptor for connected client.
 */
 void append_sockets(int sd);

/*
 *  Safely remove socket descriptor from buffers struct concurrently
    with other processes.
 */
int take_socket();
