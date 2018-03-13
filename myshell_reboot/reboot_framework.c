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

void trim_after_last(char* str, char delim)
{
    
    
    char index, last = -1;
    for(index = 0; str[index] != '\0'; index++){
        if(str[index]==delim){
            last = index;
        }
    }
    if(last >= 0){
        str[last] = '\0';
    }
    
}

void flush_tokens(char** tokens)
{
    char stringsfreed;
    for(stringsfreed = 0; tokens[stringsfreed] == NULL; stringsfreed++){
        free(tokens[stringsfreed]);
        tokens[stringsfreed] = NULL;
    }
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
    // cd for change dir is implemented much higher up. This is just pwd.
    else if(!strcmp(cmd,"cd")){
        puts(env->wd);
        return true;
    }
    else if(! strcmp(cmd,"dir")){
        dir();
        return true;
    }
    else if(! strcmp(cmd, "environ")){
        environ();
        return true;
    }
    else if(strcmp(cmd, "clr")==0 || strcmp(cmd, "clear")==0 ){
        clr();
        return true;
    }
    else if( !strcmp(cmd, "echo")){
        echo(tokens);
        return true;
    }
    
    // Out of built in commands to try. cmd isn't on the list.
    return false;
}

boolean_t external(char** tokens)
{
    // Try to find executable.
    char *cmd = tokens[0];
    char workingpath[DIRMAX + strlen(cmd) + 1];
    DIR *curdir;
    struct dirent *sd;
    curdir = opendir(workingpath);
    // Read all entries in working directory looking for cmd.
    strcpy(workingpath, env->wd);
    strcat(workingpath, "/");
    while( sd=readdir(curdir) ){
        if( !strcmp(sd->d_name, cmd) ){
            execv(cmd, tokens);
        }
    }
    // @postcondition: Couldn't find program in current wd.
    
    return true;
}

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
    // Check for directory path exceeds system max
    if(strlen(env->wd) + strlen(tokens[1]) > DIRMAX){
        puts("Exceeded maximum length of directory name.");
        return false;
    }
    
    // Look for given path.

    // Struct for current directory stream.
    DIR *curdir;
    // Struct for finding directory.
    struct dirent *sd;
    // Struct for checking state -- and type -- of entry
    struct stat entstate;
    
    // TODO: see following for trimming, so .. navigation shortens string.
    // Navigate up the tree.
    if(strcmp(tokens[1], "..")==0 || strcmp(tokens[1], "../")==0 ){
        curdir = opendir("..");
        if(curdir == NULL){
            // Navigation fails -- likely permissions error.
            puts("Can't navigate farther up.");
            return false;
        }
        // Navigated one folder up. {{Change path and continue command.}}
        trim_after_last(env->wd, '/');
        closedir(curdir);
        return true;
        
        /*
        // If command wasn't just 'move one up', continue it.
        if(tokens[1][2] != '\0'){
            char minIndex;
            for(minIndex = 0; tokens[1][minIndex] != '\0'; minIndex++){
                // Shift characters to lose first three: '../'
                tokens[1][minIndex] = tokens[1][minIndex + 3];
            }
            
            // Path is now 
            return cd(tokens);
        } 
        */
    }
    // Construct string holding abs path to target directory.
    char targetDir[DIRMAX];
    strcpy(targetDir, "/");
    strcat(targetDir, env->wd);
    strcat(targetDir, tokens[1]);
    // Try to open target.
    curdir = opendir(targetDir);
    if(curdir == NULL ){
        puts("Failed to navigate to directory.");
        return false;
    }
    strcpy(env->wd, targetDir);

    // Success. Close directory and return true.
    closedir(curdir);
    return true;
    
    /* else { // Navigating farther down directory tree.
        curdir = opendir(".");
        //puts("Searching current directory.");

        // Read all directories to find match.
        while( sd=readdir(curdir) ){
            // Check the state of this entry
            stat(sd->d_name, &entstate);
            
            
            //printf("Found directory %s. Comparing to %s.\n", sd->d_name, tokens[1]);
            if(    !strncmp(sd->d_name, tokens[1], strlen(sd->d_name)  ) ){ // found match
            
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
    } */
    
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