#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
//#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

typedef enum { false = 0, true = 1 } boolean_t;

/* *** Global Flags *** */
const char LONGESTWORD = 50;
// User ordered quit?
boolean_t quit;
// TODO: Global background flag set by tokenizer, or pass a boolean, or have exec scan for '&'?

// List of strings for PATH variables.
char** PATH;
// Path name for current working directory.
char* wd;

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

/* *** Built In Functions *** */

/*
 *  Function: cd
 *  Input:
    tokens - array of tokens
 *  If no arguments are given, print working directory (pwd).
    Else, change working directory to first argument.
 *  Return: false if unable to open requested directory, else false.

 *  Note: Can't handle recursive directories, as in "cd ./Testing/bin/debug".
    Use one level at a time.
 */
boolean_t cd(char** tokens);

/*
 *  Function: ins_hello_world
 *  Prints Hello World. Used to test deployment of processes.
 */
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
    else if(strcmp(cmd,"cd")){
        cd(tokens);
        return true;
    }
    return false;
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
    char* lastword;
    do {
        lastword = *(tokens[i++]);
    } while(! lastword[0] == '\0');
    // Find length of last token
    char len = strlen(lastword);

    return (boolean_t) lastword[len-1] == '&';
}

void ins_hello_world()
{
    const char* msg = "Hello World.";
    puts("Entered internal function.");
    puts(msg);
}

boolean_t cd(char** tokens)
{
    if(tokens[1] == void){
        // No argument. Print current working directory.
        puts(wd);
        return true;
    }
    // Look for given path.

    // Struct for current directory stream.
    DIR *curdir;
    // Struct for finding directory.
    struct dirent *sd;

    curdir = opendir(".");

    // Read all directories to find match.
    while( sd=readdir(curdir) ){
        if( !strcmp(sd->d_name, tokens[1]) ){ // found match
            strcat(wd, sd->d_name);

            closedir(curdir);
            return true;
        }
    } // Loop ended -- match not present
    puts("Directory not found.");
    return false;
}
