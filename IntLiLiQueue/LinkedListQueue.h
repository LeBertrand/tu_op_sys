#ifndef LINKEDLIST_H
#define LINKEDLIST_H

/*
 *  Node structure for linked list.
 *  Consists of pointer to next entry and data point.
 */
typedef struct Node {
    Node* next;
    int data;
} Node;

class LinkedListQueue
{
    public:
        LinkedListQueue();
        LinkedListQueue(char*);
        virtual ~LinkedListQueue();
        //LinkedList(const LinkedList& other);

        void push(int);
        int pop(void);

        unsigned int Getlength() { return length; }

    protected:

    private:
        unsigned int length;
        Node* head;
        Node* last;
        char* name;
};

#endif // LINKEDLIST_H
