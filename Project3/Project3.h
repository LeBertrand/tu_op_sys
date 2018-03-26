#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include "../csapp.c"

const char *DEFAULT_DICTIONARY = "dictionary";
const int DEFAULT_PORT = 5000;
const int NUM_WORKERS = 5;
const int Q_LEN = 20;

