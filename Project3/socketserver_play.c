#include "Project3.h"

void* showfirst(void*);
int sd[5];

int main(int argc, char *argv[])
{
    pthread_t workerthread;

    int listeningSocket = open_listenfd(5000);

    int i;
    for(i = 0; i < 5; i++){
        int connectionSocket = accept(listeningSocket, NULL, NULL);
        sd[0] = connectionSocket;
        pthread_create(&workerthread, NULL, &showfirst, connectionSocket);
    }

    return 0;
}

void *showfirst(void *varg)
{
    char buf[100];
    int connectionSocket = sd[0];
    recv(connectionSocket, buf, 100, NULL);
    printf("Received %s\n", buf);
}
