#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "open_listenfd.c"

int main(int argc, char *argv[])
{

    int socket_desc;
    struct sockaddr_in server;
    char *message, server_reply[2000];
    // Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_desc == -1)
    {
        printf("Could not create socket\n");
    }
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(5000);

    // Connect to remote server
    if( connect( socket_desc, (struct sockaddr*) &server, sizeof(server)) < 0)
    {
        puts("connect error");
        return 1;
    }

    send(socket_desc, argv[1], strlen(argv[1]), 0);
    //puts("Connected");


    return 0;
}
