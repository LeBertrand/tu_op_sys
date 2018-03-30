CC=gcc

sercle: socketserver_play.c socketclient_play.c
	make server
	make client

server: socketserver_play.c
	$(CC) socketserver_play.c -o server.out -lpthread
client: socketclient_play.c
	$(CC) socketclient_play.c -o client.out

main: main.c
	$(CC) main.c spellchecker.c -lpthread -o Project3Serv.out

# Don't bother making library out of spellchecker.c and .h.
# It's two small functions and a wrapper struct with only one component.
