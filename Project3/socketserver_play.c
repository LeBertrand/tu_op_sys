#include "Project3.h"
#include "spellchecker.h"

void* showfirst(void*);
int sd;

int main(int argc, char *argv[])
{
    pthread_t workerthread;

    int listeningSocket = open_listenfd(5000);

    puts("About to try connecting.\n");
    int connectionSocket = accept(listeningSocket, NULL, NULL);
    puts("Got connection. Talking to loopback.\n");
    sd = connectionSocket;
    puts("Put sd in *sd.About to deploy worker.\n");
    pthread_create(&workerthread, NULL, &showfirst, NULL);
    
    pthread_join(workerthread, NULL);

    return 0;
}

void *showfirst(void *varg)
{
    puts("Entered worker thread.\n");
    int status_len;
    char buf[100];
    void* status_buf = malloc(100);
    int connectionSocket = sd;
    memset(buf, '\0', 100);
    int status;
    while( recv(connectionSocket, buf, 100, NULL) ){
        //status_len = 100;
        
        
        // fflush(stdout);
        printf("Received %s\n", buf);
        memset(buf, '\0', 100);
        // getsockopt(connectionSocket, SOL_SOCKET, SO_ERROR, status_buf, &status_len);
        
        // printf("Status Buffer: %s\n", strerror(status_buf));
    }
}
