CC=gcc

cliserun: socketserver_play.c socketclient_play.c
	make server
	make client
server: socketserver_play.c
	$(CC) socketserver_play.c -o server.out -lpthread
client: socketclient_play.c
	$(CC) socketclient_play.c -o client.out
