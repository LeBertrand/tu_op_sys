#include "reboot_framework.c"
#include "shell_interface/shell_interface.h"

char** tokens;

int main()
{
    initialize();
    // Allocate buffer for user input.
    char *inputbuf = malloc(INPUT_MAX);
    // Allocate array of strings to break up user input.
    tokens = (char**) malloc(WORDS_MAX * sizeof(char*));
    // Check if string array was allocated.
    if(tokens == NULL){
        puts("Unable to allocate space for tokens.");
        exit(1);
    }
    
    // Loop until loop returns false.
    while(loop(inputbuf));
    
    // Free all allocated space.
    free(inputbuf);
    flush_tokens(tokens);
    free(tokens);
}

boolean_t loop(char* inputbuf)
{
    // Flush stdin so next get only gets new user input.
    fflush(stdin);
    // Reset tokens
    flush_tokens(tokens);
    // Print prompt.
    printf("%s >> ", env->wd);
    // Read in line of input.
    fgets(inputbuf, INPUT_MAX, stdin);
    
    // Break line of input into tokens in string array.
    tokenize(tokens, inputbuf);
    
    // Send string array to deploy_tokens, and send its return on to caller.
    return handle_command(tokens);
}

boolean_t handle_command(char** cmdtokens)
{
    // Restart loop if empty input.
    if (cmdtokens[0][0] == '\0') return true;
    
    /* Ugly solution to the inconsistent tokenizing, but it allows executables
    called quitWhileAhead.out and cdmaScanner.exe. */
    if( strncmp(cmdtokens[0], "quit", strlen(cmdtokens[0]))==0 ||
        strncmp(cmdtokens[0], "quit ", strlen(cmdtokens[0]))==0){
        return false;
    } 
    if( strncmp(cmdtokens[0], "cd", strlen(cmdtokens[0]) )==0 ||
        strncmp(cmdtokens[0], "cd ", strlen(cmdtokens[0]) )==0 ) {
        // cd works in parent process only when changing directory.
        if(tokens[1] != NULL) cd(tokens);
        return true;
    } 
    
    
    new_cmd_process(tokens);
}

boolean_t new_cmd_process(char** tokens)
{
    /* *** Handle Pipes *** */
    // If '|' in tokens:
    char index, pipeposition = -1;
    for(index = 0; tokens[index] != NULL; index++){
        if( strcmp(tokens[index], "|")==0 ){
            pipeposition = index;
            break;
        }
    }
    
    if(pipeposition > -1){
        // Make pipe and make two tokens lists.
        int Pipe[2];
        pipe(Pipe);
        // Allocate two char**, one holds pipeposition-1 real tokens and a NULL.
        char* firstTokens[pipeposition];
        for(index = 0; index < pipeposition; index++){
            firstTokens[index] = tokens[index];
        }
        firstTokens[index++] = NULL;
        
        // Find out how many tokens remain.
        for( index = 0; tokens[index + pipeposition] != NULL; index++ ) ;
        char* secondTokens[index++];
        // Other char** holds numtokens - pipeposition.
        for( index = 0; tokens[index + pipeposition] != NULL; index++ ){
            secondTokens[index] = tokens[index + pipeposition];
        }
        
        // Fork. Child gets the first command, parent does the second.
        pid_t pid = fork();
        // Check child and parent tokens lists for redirect. Reset in and out
        // streams accordingly.
        if(!pid){
            // TODO: Reset stdout.
            close(Pipe[READ]);
            dup2(Pipe[WRITE], WRITE);
            
            // TODO: Set env->PARENT with getppid
            
            new_cmd_process(firstTokens);
        }
        
        else {
            // Parent Process: Wait for Child Process.
            int status;
            wait(&status); // Not going to use status info.
            
            // Reset stdin
            close(Pipe[WRITE]);
            dup2(Pipe[READ], READ);
            
            new_cmd_process(secondTokens);
        }
        
        
    } // Pipes now handled. Rest of function should stand alone as base case.
    
    // Find redirect in. Search from [1], to skip case where first token is '<'.
    char redirectin = -1;
    for(index = 1; tokens[index] != NULL; index++){
        if( ! strcmp(tokens[index], "<") ) redirectin = ++index;
        break;
    }
    if(redirectin > -1){
        FILE* newIn = freopen(tokens[redirectin], "r", stdout);
        // TODO: Reset stdin.
    }
    
    pid_t pid = fork();
    // Execute in child process to preserve current process.
    if( !pid ) {
        // Child process calls first function.
        fprintf(stdout, "Entered only executing Child Process.\n" );
        if(!builtin(tokens)){
            external(tokens);
            puts("Unable to execute command.");
            }
    // Every process in this function:
        // Implement cd w/o args
        // Implement dir
        // Implement environ
        
        // Implement external
        // Free tokens list.
    }
    // Wait for builtin or extern to return.
    waitpid(pid, 0, 0);
    return true;
}

void tokenize(char** tokens, char* linein)
{
    // String pointer for individual tokens in linein
    char* current = strtok(linein, " ");
    
    // Index to iterate over tokens array
    int tokensread;
    
    for(tokensread = 0; current != NULL; tokensread++){
        //printf("Just read |%s|\n", current);
        // Trim trailing newline.
        if(current[strlen(current) - 1] == '\n')
            current[strlen(current) - 1] = '\0';
        tokens[tokensread] = (char*) malloc(LONGESTWORD);
        strcpy(tokens[tokensread], current);
        current = strtok(NULL, " ");
    }
    // Null terminate
    tokens[tokensread] = NULL;
}