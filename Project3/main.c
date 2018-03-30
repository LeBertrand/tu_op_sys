#include "Project3.h"

pthread_cond_t socket_requests_notfull, socket_requests_notempty;
pthread_cond_t log_queue_notfull, log_queue_notempty;
pthread_mutex_t sockets_lock, log_lock;

pthread_t *workers;
char *worker_ids;
mx_circbuf_t buffers;

int main(int argc, char *arg[])
{
    /**  Setup: **/

    /* Create Pool of Five Threads. Declarations are global in heap
    to minimize passing and help avoid losing in thread. */
    workers = (pthread_t*) malloc(NUM_WORKERS * sizeof(pthread_t));
    worker_ids = (char*) malloc(NUM_WORKERS);

    //  Initialize queue of socket descriptors for requests.
    mx_circbuf_init(&buffers, Q_LEN);

    // Create Condition variables and lock for each queue.
    pthread_cond_init(&socket_requests_notfull, NULL);
    pthread_cond_init(&socket_requests_notempty, NULL);
    pthread_cond_init(&log_queue_notfull, NULL);
    pthread_cond_init(&log_queue_notempty, NULL);
    pthread_mutex_init(&sockets_lock, NULL);
    pthread_mutex_init(&log_lock, NULL);

    //  Open Listening Socket.
    int listeningSocket = open_listenfd(5000);




    // Sit in loop producing connected sockets to clients.

//         Busy wait on socket.
//         Block on fd queue full.
//         When job comes in, put fd in queue.
//         Signal job in fd queue.
//
//
}

// Worker Threads:
//     Block on fd queue empty.
//     When signaled, take a client from queue and service.
//     Signal queue not full.
//
//     Spell Check service:
//         Read word from client.
//         Check dictionary for word.
//         Return true of false.
//         If client terminates connection, close.
//
// Logger Thread
//     Block on Jobs complete queue full.
//     When jobs complete queue not empty:
//         Get job from queue.
//         Add to log file.

void append_sockets(int sd){
    // Lock socket_requests buffer
    pthread_mutex_lock(&sockets_lock);
    // As long as buffer is full.
    while(buffers.sd_count == buffers.sd_length){
        // Block until buffer has room and then try again.
        pthread_cond_wait(&socket_requests_notfull, &sockets_lock);
    }
    // Put socket in buffer
    mx_circbuf_acceptsd(&buffers, sd);

    // Signal socket_requests not empty
    pthread_cond_signal(&socket_requests_notempty);
    // Unlock socket_requests
    pthread_mutex_unlock(&sockets_lock);
}

int take_socket()
{
    pthread_mutex_lock(&sockets_lock);
    // As long as buffer is empty
    while(buffers.sd_count == 0){
        // Block until it's not empty and then try again.
        pthread_cond_wait(&socket_requests_notempty, &sockets_lock);
    }
    // Get next socket
    int sd = mx_circbuf_offersd(&buffers);
    // Signal socket_requests not full
    pthread_cond_signal(&socket_requests_notfull);
    // Unlock socket_requests
    pthread_mutex_unlock(&sockets_lock);

    return sd;
}
