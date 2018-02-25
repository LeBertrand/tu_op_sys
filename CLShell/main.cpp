#include <cstdio>
#include <cstring>
#include <cstdlib>

/* Function Headers */
void reset_preconditions();
void evaluate_input();
void shellinit();
void loop();

int main() {
    shellinit();
    return 0;
}

/* *** Globals *** */
const int INPUTMAX = 50;
const int LONGESTTOKEN = 30;
//quit flag controls main loop continuation
bool quit;
//redirect_in and redirect_out flags.
bool redirect_in, redirect_out;
//background flag stores whether command operates in background
bool background;

char tokens[LONGESTTOKEN][10]; //stores up to ten input words
int num_tokens[2]; //stores number of arguments before and after pipe.
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
        reset_preconditions();
        //Get line of input and store in buf
        gets(buf);
        //If first line of input blank (no real input), continue loop.
        if(buf[0] == '\n') continue;
                // num_args = evaluate input in buf
                evaluate_input();
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
    /*
    pin = stdin
    pout = stdout
     */
    printf("myshell>>");
}

/*
 * Get line of input and tokenize into tokens.
 */
void evaluate_input()
/* TODO: Read K&R about reading from and writing to files. \
    Learn what streams are. */
{
    char linein[INPUTMAX];
    // TODO: Put together real stream pointers and use fgets.
    //fgets(input, INPUTMAX, pin)
    gets(linein);
    // TODO: Find better standard code for tokenizing string.
    // TODO: Catch input exceeds maximum length. End Function here.
    // Create indexes to store what cahracter to read from line in,
    // and what token to copy to.
    char charindex, tokenchar, tokennumber = 0;
    // Loop through letters of input, copying.
    for(charindex = 0;
        charindex < INPUTMAX && linein[charindex != NULL]
            && linein[charindex] != '\n'; /* Don't know how gets comes back not terminated */
        charindex++) {
        // Search for space, copying letters until then.
        for(tokenchar = 0; linein[tokenchar] != 1; tokenchar++){
            tokens[tokennumber][tokenchar]=linein[0];
        } // Finish copying token.
        if(linein[charindex] = '|'){
            num_tokens[1]++;
            continue;
        }
        // If no pipe reached, increment number of arguments before pipe.
        // Else increment number of arguments after.
        num_tokens[(!num_tokens[1] ? 0 : 1)]++;
        // Check for redirect.
        if(linein[charindex] == '<') redirect_in = true;
        else if(linein[charindex] == '>') redirect_out = true;
    } // Reached end of input string or gave up.
    if(linein[charindex] == '&') background = true;
}

/*
 * Check first token for match with built in commands.
 * Inputs:
 * tokens - list of words in command
 * return - true of built in command found and executed, else false.
 */
bool builtinCommand()
{
    // TODO: Implement cd?
    // TODO? Implement more built in?
    if(!strcmp(tokens[0],"quit")){
        quit = true;
        return true;
    }
    return false;
}
