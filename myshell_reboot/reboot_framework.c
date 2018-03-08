#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
//#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

typedef enum { false = 0, true = 1 } boolean_t;

/* *** Globals *** */
const char LONGESTWORD = 50;
const char DIRMAX = 100;

// User ordered quit?
boolean_t quit;
// TODO: Global background flag set by tokenizer, or pass a boolean, or have exec scan for '&'?

// List of strings for PATH variables.
typedef struct {
    /* TODO: What does it mean not to hardcode paths as stated in 1.i?
    Should I do a sys("pwd")?
    How am I supposed to get absolute paths to shell or to user's home? */
    char* SHELL;
    char* USER;
    char* HOME;
    char* PATH;
    // Path name for current working directory.
    char *wd;
    int wdlen;
    
    // Leave null except when deploying an external with exec.
    char* PARENT;
} ENV;

// Global pointer to ENV struct
ENV *env;

/*
 *  Function: initialize
 *  Set up environment struct.
 */
void initialize();

/*
 *  Searches for dir with given name. If directory is found, add it to PATH.
    Otherwise, print warning message.
 *  Inputs:
    newPath - string holding name of directory to search for
 *  Return: true if path found and added to PATH, false if path not found.
 */
boolean_t addtoPATH(const char* newPath);

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
 *  Function: builtin
 *  Check list of built in commands.
 *  If one matches first token, deploy it as a process.
 *  Returns: true if matching command deployed, else false.
 */
boolean_t builtin(char** tokens);
boolean_t executable(char** tokens);

/*
 *  Function: runbg
 *  Check given arguments for ampersand.
 */
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

void initialize()
{
    // Setup environment
    env = (ENV*) malloc(sizeof(ENV));
    
    // Make list of paths to search for executables.
    char* paths = (char*) malloc(100);
    strcpy(paths, "");
    strcat(paths,".");
    
    // DIR* checkdir;
    // checkdir = opendir("bin");
    // if(checkdir == NULL){
    //     printf("False PATH. Shell can't run.");
    //     exit(1);
    // }
    // else{
    //     strcat(paths,"bin:");
    //     closedir(checkdir);
    // }
    
    addtoPATH("bin");
    env->PATH = paths;
    
    env->PARENT = NULL;
    
    // TODO: Other init stuff.
    
    //TODO: How am I really supposed to get the absolute path?
    env->wd = (char*) malloc(DIRMAX);
    env->wdlen = DIRMAX;
    strcpy(env->wd, "~/workspace/tu_op_sys/myshell_reboot");
}

boolean_t safecat(char** dest, const char* addition, int* lenptr)
{
    // Get string to copy into.
    char *oldBuf = *dest;
    // Check for room. If not enough, allocate new string.
    while(strlen(addition) + strlen(oldBuf) > *lenptr + 1){
        // Create new buffer for paths.
        char* bigbuf = (char*) malloc(*lenptr * 2);
        if(bigbuf == NULL){
            return false;
        }
        // Record new paths string length.
        (*lenptr) *= 2;
        // Transfer contents of old destination string into longer string.
        strcpy(bigbuf, oldBuf);
        // Put pointer to big string wherever pointer to old string was.
        *dest = bigbuf;
    }
    // Enough space is allocated.
    // Copy.
    strcat(*dest, addition);
    
    return true;
}

boolean_t addtoPATH(const char* newPath)
{
    DIR* checkdir;
    checkdir = opendir(newPath);
    if(checkdir == NULL){
        puts("Unable to detect given path.");
        return false;
    }
    closedir(checkdir);
    safecat(&(env->wd),":",&(env->wdlen));
    safecat(&(env->wd),newPath,&(env->wdlen));
}

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
    char *lastword = tokens[i++], len;
    while( lastword != NULL ) {
        // Find length of last token.
        len = strlen(lastword);
        
        lastword = tokens[i++];
    }
    
    lastword = tokens[i-2];

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
    if(tokens[1] == NULL){
        // No argument. Print current working directory.
        puts(env->wd);
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
            strcat(env->wd, sd->d_name);
            
            // Put '/' at end of wd name so future concatenation works
            if(env->wd[strlen(env->wd) - 1] != '/'){
                strcat(env->wd, "/");
            }

            closedir(curdir);
            return true;
        }
    } // Loop ended -- match not present
    puts("Directory not found.");
    return false;
}

void environ()
{
    printf("WD=$s\n", env->wd);
    printf("PATH=%s\n", env->PATH);
    printf("PATH=%s\n", env->PARENT);
}