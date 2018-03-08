#include "reboot_framework.c"

int main()
{
    initialize();
    
    /* Check safe cat function
    char* mybuf;
    int numchars = 16;
    char temp[numchars];
    strcpy(temp, "Input");
    mybuf = temp;
    safecat(&mybuf, " here...\n", &numchars); */
    
    //safecat(&mybuf, "As soon as you find it convenient.", &numchars);
    
    /* Check that tokenizer finds ampersand
    char** tokens = (char**) malloc(4*sizeof(char*));
    char *cmd1 = (char*) malloc(LONGESTWORD),
    *cmd2 = (char*) malloc(LONGESTWORD),
    *cmd3 = (char*) malloc(LONGESTWORD),
    *cmd4 = (char*) malloc(LONGESTWORD);

    strcpy(cmd1, "logger.exe");
    strcpy(cmd2, "&");
    tokens[0] = cmd1;
    tokens[1] = cmd2;
    tokens[2] = NULL;
    if(runbg(tokens)){
        puts("Command runs in background.");
    } else {
        puts("No background command.");
    } */
    
    // Call eviron and echo functions.
    char* tokens[10];
    environ();

    return 0;
}
