#include "LinkedListQueue.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>

LinkedListQueue::LinkedListQueue()
{
    // Constructor - set up empty list
    head = NULL;
    last = NULL;

    length = 0;

    name = (char*) malloc(14);
    strcpy(name, "Unnamed Queue");
}

LinkedListQueue::LinkedListQueue(char* msg)
{
    // Constructor - set up empty list
    head = NULL;
    last = NULL;

    length = 0;

    name = (char*) malloc(14);
    if(name == NULL){
        exit(3);
    }
    strcpy(name, msg);
}

LinkedListQueue::~LinkedListQueue()
{
    while(head != NULL){
        last = head->next;
        free(head);
    }
}

void LinkedListQueue::push(int x){
    Node* insertion = (Node*) malloc(sizeof(Node));
    if(insertion == NULL){
        printf("Unable to allocate space for %s.\n", name);
    }
    insertion->data = x;
    insertion->next = NULL;
    if(head==NULL){
        head = insertion;
        last = insertion;
    } else {
        last->next = insertion;
    }
    length++;
}

int LinkedListQueue::pop()
{
    Node* take = head;
    int val = take->data;
    head = head->next;
    length--;

    free(take);
    return val;
}
