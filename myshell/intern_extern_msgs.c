#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void ins_hello_world();

int main()
{
    pid_t pid = fork();

    if(pid==0){
        puts("Child Executing");
        execlp("./EHelloWorld.exe",  "../", NULL);
    }
    else {
        waitpid(pid,0,0);
        ins_hello_world();
    }

    return 0;
}

void ins_hello_world()
{
    const char* msg = "Hello World.";
    puts("Entered internal function.");
    puts(msg);
}
