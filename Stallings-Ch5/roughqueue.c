
void queueAppend(pid_t* queue, pid_t addition){
    int i = 0;
    while(queue[i] == NULL) i++;
    queue[i] = addition;
}

pid_t queuePop(pid_t *queue){
    pid_t next = queue[0];
    int i = 0;
    while(queue[i] != NULL){
        queue[i] = queue[++i];
    }
    return next;
}

#ifndef BOOLEAN_T
#define BOOLEAN_T
typedef enum { false = 0, true = 1 } boolean_t;
#endif

boolean_t isEmpty(pid_t *queue){
    return pid_t[0] = NULL;
}
