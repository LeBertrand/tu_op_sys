#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

#ifndef BOOL_T
#define BOOL_T
typedef enum { false = 0, true = 1 } boolean_t;
#endif

/* *** Globals *** */
#ifndef SHELLCONST
#define SHELLCONST
const char LONGESTWORD = 50;
const char DIRMAX = 100;
const char INITPATHLEN = 100;
const char INPUT_MAX = 100;
const char WORDS_MAX = 20;
const char READ = 0;
const char WRITE = 1;

#endif

// User ordered quit? Flag ends main loop.
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
    int pathlen;
    // Path name for current working directory.
    char *wd;
    
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
 *  DEPREC: Declaration only preserved to allow git resets of old code. Use
            functions described in shell_interface.h.
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
 
 *  Note: Don't use directly. This function requires 'babysitting' from
 *  new_cmd_process.
 */
boolean_t builtin(char** tokens);

/*
 *  Function: external
 *  Search path for executable file match name of first token.
 *  Execute file.
 *  Note: Don't call directly. Function doesn't create new process. Call
 *  indirectly with new_cmd_process.
 */
boolean_t external(char** tokens);

/*
 *  Function: runbg
 *  Check given arguments for ampersand.
 */
boolean_t runbg(char** tokens);

/*
 *  Function: safecat
 *  Safer version of strcat. Takes pointer to location of destination pointer and
 *  allocates larger array if needed.
 *  Input:
    dest - pointer to location of destination string
    addition - string to concatenate onto string at dest
    lenptr - pointer to variable storing length of dest string.
 */
boolean_t safecat(char** dest, const char* addition, int* lenptr);

/*
 *  Function: trim_after_last
 *  Modifies str by trimming everything after last occurence of delim, unless
 *  delim isn't in str. In that case, no change is made.
 *  Input:
    delim - character whose last occurrence indicates place to trim.
    Output:
    str - string to trim
 */
void trim_after_last(char* str, char delim);

/*
 *  Function: flush_tokens
 *  Free space allocated to all strings in array.
 */
void flush_tokens(char**);

/* *** Built In Functions *** */

/*
 *  Function: cd
 *  Input:
    tokens - array of tokens
 *  If no arguments are given, print working directory (pwd).
    Else, change working directory to first argument.
 *  Return: false if unable to open requested directory, else true.

 *  Note: Can't handle recursive directories, as in "cd ./Testing/bin/debug".
    Use one level at a time.
 */
boolean_t cd(char** tokens);

/*
 *  Function: dir
 *  Lists all contents of current directory.
 */
void dir();

/*
 *  Function: environ
 *  Print list of environment variables to process's output.
 */
void environ();

/*
 *  Function: echo
 *  Input:
    tokens - array of strings, terminated by NULL pointer
 *  Print all input to process's output.
 
 *  Notes:
 *  Doesn't take switches as standard UNIX echo does. Just outputs the
    exact input.
 *  Prints all tokens after the first, seperated by spaces.
 */
void echo(char** tokens);

/*
 *  Function: clr
 *  Clear terminal.
 */
void clr();

/*
 *  Function: ins_hello_world
 *  Prints Hello World. Used to test deployment of processes.
 */
void ins_hello_world();
