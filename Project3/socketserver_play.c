#include "Project3.h"
#include "spellchecker.h"

void* showfirst(void*);
int *sd;

int main(int argc, char *argv[])
{
    pthread_t workerthread;

    int listeningSocket = open_listenfd(5000);

    puts("About to try connecting.\n");
    int connectionSocket = accept(listeningSocket, NULL, NULL);
    puts("Got connection. Talking to loopback.\n");
    sd = &connectionSocket;
    puts("Put sd in *sd.About to deploy worker.\n");
    pthread_create(&workerthread, NULL, &showfirst, NULL);

    return 0;
}

void *showfirst(void *varg)
{
    puts("Entered worker thread.\n");
    char buf[100];
    memset(buf, '\0', 100);
    int connectionSocket = sd[0];
    recv(connectionSocket, buf, 100, NULL);
    fflush(stdout);
    printf("Received %s\n", buf);
}
