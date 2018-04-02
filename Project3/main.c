#include "Project3.h"

pthread_cond_t socket_requests_notfull, socket_requests_notempty;
pthread_cond_t log_queue_notfull, log_queue_notempty;
pthread_mutex_t sockets_lock, log_lock;

// Declare location for array of workers.
pthread_t *workers;
// Declare location for array of worker IDs
char *worker_ids;
// Declare struct holding buffers and concurrency tools for sockets and log.
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
    int connectionSocket, listeningSocket = open_listenfd(5000);
    
    // Deploy workers
    char i;
    for(i = 0; i < NUM_WORKERS; i++){
        int tid = pthread_create(&workers[i], NULL, &worker_routine, NULL);
        worker_ids[i] = i;
    }
    
    // Deploy logger
    pthread_t loggerthread;
    void *varg;
    pthread_create(&loggerthread, NULL, &logger_routine, varg);
    
    // Sit in loop producing connected sockets to clients.
    while(true){
        connectionSocket = accept(listeningSocket, NULL, NULL);
        append_sockets(connectionSocket);
    }
    
    for(i = 0; i < NUM_WORKERS; i++){
        pthread_join(workers[i], NULL);
    }
    
    pthread_join(loggerthread, NULL);
    
    // Shouldn't ever display. Indicates loops broke.
    puts("\n\nGood run. Closing down...");
    return 1;
}


// Worker Threads:
void *worker_routine(void *varg)
{
    // Variable holds connected socket taken from shared socket_requests buffer.
    int connectionSocket;
    
    // Setup dictionary within thread's own local varspace. Dictionary isn't shared.
    Wordslist_t wl = spellcheck_initialize(DEFAULT_DICTIONARY);
    
    // Allocate string to read into from socket.
    char local_buf[100];
    memset(local_buf, '\0', 100);
    
    // Main Loop -- receiving and spell checking
    while(true){
        
        // Safe get socket from list of connections
        connectionSocket = take_socket();
    
        // ** DBRL ** Report thread ID and claim connection.
        // printf("Thread #%d claimed connection.\n", (int) pthread_self());
        
        // Continue reading as long as connection exists.
        //         Read word from client.
        while( recv(connectionSocket, local_buf, 100, 0) ){
        //     Spell Check service:
            // Ignore blank line
            if( *local_buf == '\n' ) {
                continue;
            } 
            //Trim newline char
            if( local_buf[ strlen(local_buf) - 1 ] == '\n'){
                local_buf[ strlen(local_buf) - 1 ] = '\0';
            }
            // Check dictionary for word.
            if( spellcheck_searchword(&wl, local_buf) ){
                strcat(local_buf, " OK\n");
            } else {
                strcat(local_buf, " MISSPELLED\n");
            }
            
            // Send reply
            send(connectionSocket, local_buf, strlen(local_buf), 0);
            
            // Put reply in log queue
            append_lookups(local_buf);
        
            memset(local_buf, '\0', 100);
        }
        
    }
    
}

void* logger_routine(void *varg)
{
    
    char *msg;
    while(msg = take_lookup()){
        FILE *log = fopen("SpellCheckLog.txt", "a");
        fprintf(log, msg);
        free(msg);
        fclose(log);
    }
}


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

void append_lookups(char *lookupmsg)
{
    char *msgspace = (char*) malloc(strlen(lookupmsg));
    strcpy(msgspace, lookupmsg);
    pthread_mutex_lock(&log_lock);
    // As long as log queue is full when checked...
    while(buffers.msg_count == buffers.msg_length){
        // Block until signal that log_queue notfull.
        pthread_cond_wait(&log_queue_notfull, &log_lock);
    }
    // Put lookupmsg into log message buffer
    mx_circbuf_acceptmsg(&buffers, msgspace);
    
    // Signal log_msg not empty
    pthread_cond_signal(&log_queue_notempty);
    // Unlock log_msgs
    pthread_mutex_unlock(&log_lock);
}

char *take_lookup()
{
    pthread_mutex_lock(&log_lock);
    // As long as buffer is empty
    while(buffers.msg_count == 0){
        // Block until it's not empty and then try again.
        pthread_cond_wait(&log_queue_notempty, &log_lock);
    }
    // Get next log msg
    char* msg = mx_circbuf_offermsg(&buffers);
    // Signal log msgs not full
    pthread_cond_signal(&log_queue_notfull);
    // Unlock socket_requests
    pthread_mutex_unlock(&log_lock);
    
    return msg;
}