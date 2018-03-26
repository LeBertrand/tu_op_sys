#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
//#include "open_listenfd.c"
#include "../csapp.c"

void* showfirst(void*);

int main(int argc, char *argv[])
{
    pthread_t workerthread;

    int listeningSocket = open_listenfd(5000);

    int i;
    for(i = 0; i < 5; i++){
        int connectionSocket = accept(listeningSocket, NULL, NULL);
        pthread_create(workerthread, NULL, &showfirst, connectionSocket);
    }

    return 0;
}

void *showfirst(void *connectionSocket)
{
    char buf[100];
    recv(connectionSocket, buf, 100, NULL);
    printf("Received %s\n", buf);
}
