#include <cstdio>
#include <cstring>
#include <sys/types.h>
#include <unistd.h>
#include <cstdlib>

/* Function Headers */
void reset_preconditions();
void tokenize_input();
void deploy_processes();
void shellinit();
void loop();
bool builtinCommand(char**);
/*
 * Handle Pipe
 *
 * Function called if pipe detected.
 * Just deploys other processes by calling deploy processes.
 */
void handle_pipe(void);

/* *** Globals *** */
const int INPUTMAX = 50;
const int LONGESTTOKEN = 30;
//quit flag controls main loop continuation
bool quit;
// Record whether current call has pipe - convenient redundancy for num_tokens[1]
bool pipe_called;
//background flag stores whether command operates in background
bool background;
//redirect_in and redirect_out flags, store unhandled redirect status.
bool redirect_in, redirect_out;

char *tokens[10]; //stores up to ten input words
int num_tokens[2]; //stores number of arguments before and after pipe.
// TODO: How do I declare pointers to PATH and to streams? Is PATH just stored as a string?
//PATH**
FILE* pin = stdin;
// TODO: How do I declare pointers to PATH and to streams? Is PATH just stored as a string?
//PATH**
FILE *pout = stdout; //pointers for redirection...all input and output refer to them.

/*
 * TODO: Summarize main function.
 */
int main()
{
    shellinit();
    reset_preconditions();

    return 0;
}

/* Initialization */
void shellinit()
{
    quit = false;
    //set tokens 1:10 NULL
    int i;
    for(i = 0; i < 10; i++){
        free(tokens[i]);
        tokens[i] = NULL;
    }
    // TODO: Set PATH to directories with executables.
}


/* Main Loop */
void loop()
{
    char buf[INPUTMAX];
    // While not quit:
    while(!quit){
        reset_preconditions();
        printf("myshell>>");
        //Get line of input and store in buf
        fgets(buf, INPUTMAX, stdin);
        //If first line of input blank (no real input), continue loop.
        if(buf[0] == '\n') continue;
                // num_args = evaluate input in buf
                tokenize_input();
        //done = Built in command
        //if not done, done = External Executable.
        //        If not done print default message. */
    }
}

void reset_preconditions()
{
    // Flush input
    fflush(stdin);
    // Command is presumed foreground. Background false until '&' read.
    background = false;
    if(redirect_in){
        fclose(pin);
        pin = stdin;
        redirect_in = false;
    }
    if(redirect_out) {
        fclose(pout);
        pout = stdout;
        redirect_out = false;
    }
}

/*
 * Get line of input and tokenize into tokens 2D array.
 */
void tokenize_input()
/* TODO: Read K&R about reading from and writing to files. \
    Learn what streams are. */
{
    char linein[INPUTMAX];
    // TODO: Put together real stream pointers and use fgets.
    //fgets(input, INPUTMAX, pin)
    fgets(linein, LONGESTTOKEN, stdin);
    // TODO: Find better standard code for tokenizing string.
    // TODO: Catch input exceeds maximum length. End Function here.
    // Create indexes to store what character to read from line in,
    // what position in the current token, and what token to copy to.
    char charindex, tokenchar, tokennumber = 0;
    // Loop through letters of input, copying.
    for(charindex = 0;
        charindex < INPUTMAX && linein[charindex] != NULL
            && linein[charindex] != '\n'; /* Don't know how gets comes back not terminated */
        charindex++) {

        // Create string to store token.
        char* tokenc;

        // Search for space, copying letters until space.
        for(tokenchar = 0; linein[tokenchar] != ' '; tokenchar++){

            tokenc[tokenchar]=linein[0];
        } // Finish copying token.

        if(linein[charindex] = '|'){
            num_tokens[1]++;
            pipe_called = true;
            continue;
        } // End of word
        // If no pipe reached, increment number of arguments before pipe.
        // Else increment number of arguments after.
        num_tokens[(!num_tokens[1] ? 0 : 1)]++;
        // Check for redirect.
        if(linein[charindex] == '<') redirect_in = true;
        else if(linein[charindex] == '>') redirect_out = true;
        else if(redirect_in){
            pin = fopen(tokens[tokennumber], "r");
        }
        else if(redirect_out){
            pout = fopen(tokens[tokennumber], "w");
        }
        tokennumber++;
    } // Reached end of input string or gave up.
    if(linein[charindex] == '&') background = true;
}


/*
 * Take tokens after tokenize_input reads them in.
 * Deploy necessary processes with correct IO pointers and argument lists.
 *
 * Note: Earlier documentation included this function and tokenize_input in one
 * 'evaluate_input' function.
 * In order to make functions concise, the two have been seperated. This function
 * manages distinct processes.
 */
    void deploy_processes(int tokensoffset)
    {
        pid_t pid = fork();
        if(!pid){
            if(redirect_out){
                stdout = pout;
                if(!builtinCommand(tokens + LONGESTTOKEN * tokensoffset)){
                    // Call external functions
                }

            }
        }
        /*
        if(pipe_called){
            pipe_t = pipe()
            pid_t pid = fork();
        }
        if(pid==0){
            //Child process tries to execute built in command.

        }
         */
    }

/*
 * Check first token for match with built in commands.
 * Inputs:
 * tokens - list of words in command
 * return - true of built in command found and executed, else false.
 *
 * Takes pointer to tokens to allow passing only end of list to piped process.
 */
    bool builtinCommand(char** tokens)
    {
        // TODO: Implement cd?
        // TODO? Implement more built in?
        if(!strcmp(tokens[0],"quit")){
            quit = true;
            return true;
        }
        return false;
    }

void handle_pipe() {
    if (num_tokens[1]) {
// Open pipe.
        int fd[2];
        pipe(fd);
        pid_t pid = fork();
        if (!pid) {
            // Write to pipe.
            deploy_processes(0);
// Child process executes until pipe and writes to it.
        } else {
// Wait for child process and reads from pipe.
            deploy_processes(num_tokens[1]);
        }
    }
}

// TODO: Cleanup function