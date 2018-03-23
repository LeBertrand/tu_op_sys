#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "open_listenfd.c"

int main()
{
    char *buf = (char *) malloc(100);
    strcpy(buf, "Message for socket below.\nEnd message.\n");

    int sd = open_listenfd(90);
    int connectionSocket = accept(sd, NULL, NULL);
    
    puts("Client sending message");
    send(sd, buf, strlen(buf), 0);
    
    return 0;
}
