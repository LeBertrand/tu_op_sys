#ifndef BOOL_T
#define BOOL_T
typedef enum { false = 0, true = 1 } boolean_t;
#endif

#ifndef SHELLCONST
#define SHELLCONST
const char LONGESTWORD = 50;
const char DIRMAX = 100;
const char INITPATHLEN = 100;
const char WORDS_MAX = 20;
#endif

static int uni_stdin;
static int uni_stdout;

/*
 *  Function: tokenize
 *  Input:  inputline - String read in from standard input
 *  Output: tokens - Array of strings
 *  Break input at spaces and non-alphanumerics. Put words into character array.
 */
void tokenize(char** tokens, char* inputline);

/*
 *  Function: loop
 *  Main function calls functions to get input and then to deploy processes.
 *  Output:
    inputbuf - store user input, read from stdin
 *  Returns: false if shell should exit, true otherwise.
 
 *  Use return to control loop ... while(loop())
 */
 boolean_t loop(char* inputbuf);
 
 /*
  * Function: handle_command
  * Read through tokens and deploy correct processes.
  * cd is run in parent process.
  * Last function to handle anything in parent process. Commands delegated are
  * sent to a new process.
  */
 boolean_t handle_command(char** tokens);
 
/*
 *  Function: new_cmd_process
 *  Create new process, scan through tokens for redirects, and execute.
 *  First look for pipe, establish pipe, and call itself recursivesly in seperate
 *  processes for commands before and commands after pipe.
 *  Then look for redirect and reset in and out streams.
 *  Try to call commands, first as builtins, then as external commands.
 */
boolean_t new_cmd_process(char** tokens);