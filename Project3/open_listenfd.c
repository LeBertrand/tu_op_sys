/*
 *  Code taken from Bryant O'Hallaron page 906.
 */

int open_listenfd(int port)
{
    int listenfd, optval=1;
    struct sockaddr_in serveraddr;
    
    /* Create a socket descriptor */
    if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return -1;
    
    /* Eliminates "Address already in use" error from bind */
    if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,
                    (const void*)&optval, sizeof(int)) < 0)
        return -1;
    
    /* Listenfd will be an end point for all requests to port on any IP address
        for this host. */
    bzero((char*) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons((unsigned short)port);
    if(bind(listenfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0)
        return -1;
        
    /* Make if a listening socket ready to accept connection requests */
    if(listen(listenfd, 1024) < 0)
        return -1;
    return listenfd;
}