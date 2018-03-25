#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
//#include "open_listenfd.c"
#include "../csapp.c"

/*
const int BUFLEN = 100;
int main()
{
    char *buf = (char *) malloc(BUFLEN); // Buffer reads from file.
    int lsckt = open_listenfd(90);
    int connectionSocket = accept(lsckt, NULL, NULL);

    recv(connectionSocket, buf, BUFLEN, 0);

    printf("Server Read in --\n%s\n", buf);

    return 0;
}
*/

int main(int argc, char *argv[])
{
    int listeningSocket = open_listenfd(5000);
    int i;
    for(i = 0; i < 5; i++){
        int connectionSocket = accept(listeningSocket, NULL, NULL);
    }
    
    return 0;
}
