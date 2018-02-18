CC=gcc

CFLAGS=-c -Wall -fPIC -I.

CLEANUP=rm -rf *.o

all: makeLink

makeLink: makeLib
	ln -sf libArr.so.0.0 libArr.so.0
	ln -sf libArr.so.0 libArr.so

makeLib: LinkedListQueue.o

LinkedListQueue.o: LinkedListQueue
