#include "reboot_framework.c"
#include "shell_interface/shell_interface.h"

char** tokens;

int main(int argc, char* argv[])
{
    FILE *logger = fopen("runlog", "w");
    fprintf(logger, "Initialized Program. PID - %d\n", getpid());
    fclose(logger);

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
    
    // Loop until loop returns false. If batchfile was given, loop on it.
    if(argc > 1){
        FILE *batchfile = fopen(argv[1], "r");
        while(batchloop(inputbuf, batchfile));
    }
    else {
        // No batchfile. Loop on terminal input.
        while(loop(inputbuf));
    }
    // Free all allocated space.
    free(inputbuf);
    flush_tokens(tokens);
    free(tokens);
}

boolean_t batchloop(char* inputbuf, FILE* batchfile)
{
    // Save standard output.
    // uni_stdout = dup(1);
    // Set in to read from batchfile.
    FILE *defaultin = batchfile;
    // If can't read batchfile, quit.
    if(defaultin == NULL){
        puts("Can't read batchfile.");
        return false;
    }
    // Prepare tokens array.
    flush_tokens(tokens);
    
    // Read in line of input.
    fgets(inputbuf, INPUT_MAX, defaultin);
    // Break line of input into tokens in string array.
    tokenize(tokens, inputbuf);
    /* Call command and hold return.
    Return yes (continue) until hitting quit or exit or running out of input. */
    boolean_t again = handle_command(tokens) && !feof(defaultin);
    // Output line break
    //putchar('\n');
    
    // Was input redirected? Set it back to batchfile.
    if(defaultin != batchfile){
        defaultin = batchfile;
    }
    // TODO: Was output redirected? Reset.
    
    
    return again;
}

boolean_t loop(char* inputbuf)
{
    // Store system in and out defaults to allow later reset.
    // uni_stdin = dup(0);
    // uni_stdout = dup(1);
    
    // Print prompt.
    printf("%s >> ", env->wd);
    
    // Reset tokens
    flush_tokens(tokens);
    // Read in line of input.
    fgets(inputbuf, INPUT_MAX, stdin);
    
    // Break line of input into tokens in string array.
    tokenize(tokens, inputbuf);
    
    // Reset standard in and out.
    // fclose(stdin);
    // stdin = uni_stdin;
    // stdout = uni_stdout;
    
    FILE* runlog = fopen("runlog", "a");
    fprintf(runlog, "Back in main loop.\n");
    fprintf(runlog, "Proc - %d\n", getpid());
    fclose(runlog);
    
    /*
    // Flush stdin so next get only gets new user input.
    //fflush(stdin); undefined (crashing)
    char c;
    while( (c = getchar()) != '\n' && c != EOF ) {} ; */
    
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
    // Ugly bit: cd directly--no fork.
    if( strncmp(cmdtokens[0], "cd", strlen(cmdtokens[0]) )==0 ||
        strncmp(cmdtokens[0], "cd ", strlen(cmdtokens[0]) )==0 ) {
            
        // printf("handle_command function reached cd block with cmd %s\n", cmdtokens[0]); /* DBRL */
        // cd works in parent process only when changing directory.
        if(cmdtokens[1] != NULL && cmdtokens[1][0] != '<' && cmdtokens[1][0] != '>') {
            new_cmd_process(cmdtokens);
            return true;
        }
    } 
    
    // TODO: Put fork back here, so runbg is checked and controls flow.
    pid_t cmdpid = fork();
    if( !cmdpid ) {
        new_cmd_process(tokens);
        
        exit(0);
    }
    
    if( ! runbg(tokens) ) {
        waitpid(cmdpid, 0, 0);
    }
    
    FILE* runlog = fopen("runlog", "a");
    fprintf(runlog, "Back to parent process, returning to loop.\n");
    fprintf(runlog, "Proc - %d\n", getpid());
    fclose(runlog);
    // dup2(uni_stdin, READ);
    // dup2(uni_stdout, WRITE);
    
    return true;
}

boolean_t new_cmd_process(char** tokens)
{
    /* *** Handle Pipes *** */
    // If '|' in tokens, find out where.
    char index, pipeposition = -1;
    for(index = 0; tokens[index] != NULL; index++){
        if( strcmp(tokens[index], "|")==0 ){
            pipeposition = index;
            break;
        }
    }
    
    if(pipeposition > -1){
        // Make pipe and make tokens list.
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
        
        /* We're discarding all further input, since input has been redirected.
        char* secondTokens[index++];
        // Other char** holds numtokens - pipeposition.
        for( index = 0; tokens[index + pipeposition] != NULL; index++ ){
            secondTokens[index] = tokens[index + pipeposition];
        } */
        
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
            waitpid(pid,0,0);
            
            // Reset stdin
            close(Pipe[WRITE]);
            
            char buf[1000];
            // Get input from pipe.
            read(Pipe[READ], buf, 1000);
            tokenize(tokens, buf);
            
            new_cmd_process(tokens);
        }
        
        
    } // Pipes now handled. Rest of function should stand alone as base case.
    
    // Find redirect in. Search from [1], to skip case where first token is '<'.
    int redirectin = -1;
    for(index = 1; tokens[index] != NULL; index++){
        if( ! strcmp(tokens[index], "<") ) {
            redirectin = ++index;
            break;
        }
    }
    if(redirectin > -1){
        FILE* newIn = freopen(tokens[redirectin], "r", stdin);
        
        char buf[1000];
        // Get input from source.
        fgets(buf, 1000, newIn);
        // Lose all tokens except the first.
        tokenize(&tokens[1], buf);
        
        new_cmd_process(tokens );
        
        return true;
    }
    
    // Find redirect out. Search from [1], to skip case where first token is '>'.
    // modestr string stores either "w" or "a" to indicate whether to append.
    char modestr[] = { 'w', '\0' } ;
    int redirectout = -1;
    
    for(index = 1; tokens[index] != NULL; index++){
        if( *(tokens[index]) == '>' ){
            redirectout = ++index;
            if(tokens[index][1] == '>') modestr[0] = 'a';
            break;
        }
    }
    if(redirectout > -1){
        FILE* newOut = freopen(tokens[redirectout], modestr, stdout);
        // dup2(*newOut, WRITE);
        puts("Reset out."); /*DBRL*/
    }
    
    //pid_t pid = fork(); Don't fork here--fork in handle_command
    // Execute in child process to preserve current process.
    // Child process calls first function.
    // fprintf(stdout, "Entered only executing Child Process.\n" );
    if(!builtin(tokens)){
        // puts("Builtin came back false. Trying external.");
        external(tokens);
        puts("Unable to execute command.");
    }
    
    // FILE* runlog = fopen( "runlog", "a"); /* DBRL */
    // fprintf(runlog, "Finished builtin-external-default. Reaching exit.\n"); /* DBRL */
    // fprintf(runlog, "Proc - %d\n", getpid()); /* DBRL */
    // fclose(runlog); /* DBRL */
    // exit(0);

    // // Parent wait for builtin or extern to return.
    // // waitpid(pid, 0, 0);
    // FILE* runlog = fopen("runlog", "a"); /* DBRL */
    // fprintf(runlog, "Exited forked process in new_cmd_process. Returning true.\n"); /* DBRL */
    // fprintf(runlog, "Proc - %d\n", getpid()); /* DBRL */
    // fclose(runlog); /* DBRL */ 
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