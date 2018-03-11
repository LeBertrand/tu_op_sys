#include "reboot_framework.c"

int main()
{
    initialize();
    char** tokens = (char**) malloc(4*sizeof(char*));
    
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
    
    //environ();

    /* cd
    char cmd[30], cmd1[30], cmd2[30];
    
    strcpy(cmd,"cd");
    strcpy(cmd1,"cmake-build-debug");
    strcpy(cmd2,"reboot_tests.c.o");
    
    tokens[0] = cmd;
    tokens[1] = cmd2;
    cd(tokens);
    
    tokens[1] = NULL;
    cd(tokens);
    
    
    tokens[1] = cmd1;
    cd(tokens);
    
    tokens[1] = NULL;
    cd(tokens); */
    
    /* clr
    puts("Fill the screen with all \nthis text...\n\nhere\nhere");
    char g,h,i,j;
    scanf("%c", &g);
    clr();
    puts("Screen clear.");
    scanf("%c%c%c%c", &g,&h,&i,&j); */
    
    //puts(env->SHELL);
    
    /* echo
    char cmd[30], cmd1[30], cmd2[30];
    
    strcpy(cmd,"echo");
    strcpy(cmd1,"cmake-build-debug");
    strcpy(cmd2,"reboot_tests.c.o");
    
    tokens[0] = cmd;
    tokens[2] = cmd1;
    tokens[1] = cmd2;
    
    echo(tokens); */
    
    dir();
    
    return 0;
}

