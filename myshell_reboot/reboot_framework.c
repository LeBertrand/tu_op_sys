#include "reboot_framework.h"

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
    char* paths = (char*) malloc(INITPATHLEN);
    strcpy(paths, "");
    strcat(paths,"./");
    
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
    
    // Set SHELL to absolute location of shell executable
    env->SHELL = (char*) malloc(DIRMAX);
    getcwd(env->SHELL, DIRMAX);
    // If failed to find directory, exit.
    if(env->SHELL == NULL){
        puts("Shell directory name overflows allocated buffer.");
        exit(1);
    }
    // Set working directory.
    env->wd = (char*) malloc(DIRMAX);
    strcpy(env->wd,env->SHELL); 
    // Working directory name ends with '/' for future concatenation.
    if(env->wd[strlen(env->wd) - 1] != '/'){
        strcat(env->wd, "/");
    }
    // TODO: name executable file 'myshell'.
    int plen = strlen(env->SHELL);
    safecat(&(env->SHELL), "myshell", &plen);

    // Put path string into env struct.
    env->PATH = paths;
    env->pathlen = INITPATHLEN;
    addtoPATH("bin");
    
    env->PARENT = NULL;
    
    // Set quit false.
    quit = false;
    
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
    safecat(&(env->PATH),":",&(env->pathlen));
    safecat(&(env->PATH),newPath,&(env->pathlen));
}

boolean_t builtin(char** tokens)
{
    char* cmd = tokens[0];
    if(!strcmp(cmd,"quit")){
        quit = true;
        return true;
    }
    else if(!strcmp(cmd,"cd")){
        cd(tokens);
        return true;
    }
    else if(! strcmp(cmd,"dir")){
        dir();
    }
    else if(! strcmp(cmd, "environ")){
        environ();
    }
    else if(strcmp(cmd, "clr")==0 || strcmp(cmd, "clear")==0 ){
        clr();
    }
    else if( !strcmp(cmd, "echo")){
        echo(tokens);
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
    // Struct for checking state -- and type -- of entry
    struct stat entstate;

    curdir = opendir(".");
    //puts("Searching current directory.");

    // Read all directories to find match.
    while( sd=readdir(curdir) ){
        // Check the state of this entry
        stat(sd->d_name, &entstate);
        
        
        //printf("Found directory %s. Comparing to %s.\n", sd->d_name, tokens[1]);
        if( !strcmp(sd->d_name, tokens[1]) ){ // found match
        
            // Check if match is a directory
            if(!S_ISDIR(entstate.st_mode)){
                printf("Can't enter %s -- not a directory.\n", sd->d_name);
                return false;
            }
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

void dir(){
    // Struct stores Directory.
    DIR *curdir;
    // Struct represents last entry read from current directory.
    struct dirent *sd;
    // Struct represents information about sd.
    struct stat curstat;

    // Open current directory.
    curdir = opendir(env->wd);

    if(curdir == NULL){
        puts("Failed to open directory.");
        exit(1);
    }
    
    while( sd=readdir(curdir)){
        stat(sd->d_name, &curstat);
        if(S_ISDIR(curstat.st_mode))
            printf("Folder -- %s...\n", sd->d_name);
        else
            printf("Entry -- %s...\n", sd->d_name);
    }
}

void environ()
{
    printf("WD=%s\n", (env->wd));
    printf("PATH=%s\n", env->PATH);
    if( env->PARENT != NULL)
        printf("PATH=%s\n", (char*) env->PARENT);
}

void clr()
{
    printf("\33[H\33[2J");
}

void echo(char** tokens){
    int i;
    for(i = 1; tokens[i] != NULL; i++){
        printf("%s ", tokens[i]);
    }
}