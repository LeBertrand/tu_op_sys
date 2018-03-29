struct binary_semaphore {
    enum { zero, one } value;
    pid_t *queue;
};

void semWait(binary_semaphore s)
{
    if(s.value == one){
        s.value = zero;
    } else {
        queueAppend(s.queue, getpid());
        wait(&(int status));
    }
}

void semSignal(binary_semaphore s);
{
    if(isEmpty(s.queue)){
        s.value = one;
    } else {
        pid_t P = queuePop(s.queue);
        /* TODO: Place P on ready list */
    }
}
