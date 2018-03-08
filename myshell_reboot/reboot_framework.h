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
 *  Function: environ
 *  Print list of environment variables to process's output.
 */
void environ();

/*
 *  Function: echo
 *  Print arguments to process's output.
 */
void echo(char** tokens);

/*
 *  Function: ins_hello_world
 *  Prints Hello World. Used to test deployment of processes.
 */
void ins_hello_world();