CC=gcc

cliserun: socketserver_play.c socketclient_play.c
	server
	client
server: socketserver_play.c
	$(CC) socketserver_play.c -o server.out
client: socketclient_play.c
	$(CC) socketclient_play.c -o client.out
