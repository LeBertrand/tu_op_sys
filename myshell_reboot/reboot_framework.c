#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
//#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

typedef enum { false = 0, true = 1 } boolean_t;

/* *** Global Flags *** */
const char LONGESTWORD = 50;
// User ordered quit?
boolean_t quit;
// TODO: Gloabal background flag set by tokenizer, or pass a boolean, or have exec scan for '&'?

void initialize();
void findpath();

/*
 *  Function: tokenize
 *  Input:  inputline - String read in from standard input
 *  Output: tokens - Array of strings
 *  Break input at spaces and non-alphanumerics. Put words into character array.
 */
void tokenize(char** tokens, char* inputline);

/*
 *  Function: deployprocess
 *  Input:  tokens - Array of strings holding input commands and arguments
 *  Check if command is built in and call correct function. If command isn't
    built in, try to execute external program. If that fails, display error message.
 */
void deployprocess(char** tokens);

/*
 *
 */
boolean_t builtin(char** tokens);
boolean_t executable(char** tokens);
boolean_t runbg(char** tokens);

void ins_hello_world();

/*
int main()
{
    return 0;
} */

boolean_t builtin(char** tokens)
{
    char* cmd = tokens[0];
    if(!strcmp(cmd,"quit")){
        quit = true;
        return true;
    }
    else if(strcmp(cmd,"hw")){
        ins_hello_world();
        return true;
    }
}
/*
boolean_t external(char** tokens)
{
    pid_t pid = fork();

    boolean_t bg = runbg(tokens);

    if(pid == 0){
        char cmd[LONGESTWORD];
        strcpy(cmd, "./");
        strcat(cmd, tokens[0]);
        execl(cmd, tokens[0], &tokens[1]);
    }
    else{
        if(!bg) waitpid(pid,NULL,NULL);
    }
    return true;
} */

boolean_t runbg(char** tokens)
{
    // Find last token
    int i = 0;
    char *lastword, len;
    while(true) { // loop until reaching an unset token.
        
        if(tokens[i] != NULL){
            lastword = tokens[i++];
            // Find length of last token
            len = strlen(lastword);
        }
        else break;
    }

    return (boolean_t) lastword[len-1] == '&';
}

void ins_hello_world()
{
    const char* msg = "Hello World.";
    puts("Entered internal function.");
    puts(msg);
}
