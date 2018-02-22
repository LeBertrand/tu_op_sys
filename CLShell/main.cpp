#include <cstdio>
#include <cstring>

int main() {
    return 0;
}

/* *** Globals *** */
const int INPUTMAX = 50;
//quit flag controls main loop continuation
bool quit;
//redirect_in and redirect_out flags.
bool redirect_in, redirect_out;
//background flag stores whether command operates in background
bool background;

char* tokens[10]; //stores up to ten input words
int num_args[2]; //stores number of arguments before and after pipe.
// TODO: How do I declare pointers to PATH and to streams? Is PATH just stored as a string?
/* <WN>
PATH**
pin, pout pointers for redirection // all input and output refer to them. */

/* Initialization */
void shellinit()
{
    quit = false;
    //set tokens 1:10 NULL
    int i;
    for(i = 0; i < 10; i++){
        strcpy(tokens[i], NULL);
    }
    // TODO: Set PATH to directories with executables.
}


/* Main Loop */
void loop()
{
    char buf[INPUTMAX];
    // While not quit:
    while(!quit){

        //Get line of input and store in buf
        gets(buf);
        //If first line of input blank (no real input), continue loop.
        if(buf[0] == '\n') continue;
                // num_args = evaluate input in buf
                evaluate_input();
        done = Built in command
        if not done, done = External Executable.
                If not done print default message. */
    }
}

void reset_preconditions()
{
    // Flush input
    fflush(stdin);
    // Command is presumed foreground. Background false until '&' read.
    background = false;
    /*
    pin = stdin
    pout = stdout
     */
    printf("myshell>>");
}

void evaluate_input()
{

}
/*
