#include "reboot_framework.c"
#include "shell_interface.h"
//#include "shell_interface/tokenize.c"

int main()
{
    //initialize();
    //char** tokens = (char**) malloc(4*sizeof(char*));
    //char linein[100];
    //strcpy(linein,"cat fluids | wc -a > calulation.txt | grep \"-v\" ");
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
    
    //dir();
    
    /*
    tokenize(tokens, linein);
    char i;
    for(i = 0; tokens[i] != NULL; i++){
        puts(tokens[i]);
    } */
    
    char* list[10];
    char* cmd1;
    cmd1 = (char*) malloc(10);
    strcpy(cmd1, "First");
    list[0] = cmd1;
    char *cmd2;
    cmd2 = (char*) malloc(10);
    strcpy(cmd2, "Second");
    list[1] = cmd2;
    char *cmd3;
    cmd3 = (char*) malloc(10);
    strcpy(cmd3, "Third");
    list[2] = cmd3;
    char *cmd4;
    cmd4 = (char*) malloc(10);
    strcpy(cmd4, "Fourth");
    list[3] = cmd4;
    char *cmd5;
    cmd5 = (char*) malloc(10);
    strcpy(cmd5, "Fifth");
    list[4] = cmd5;
    char *cmd6;
    cmd6 = (char*) malloc(10);
    strcpy(cmd6, "Sixth");
    list[5] = cmd6;
    char *cmd7;
    cmd7 = (char*) malloc(10);
    strcpy(cmd7, "Seventh");
    list[6] = cmd7;
    char *cmd8;
    cmd8 = NULL;
    //strcpy(cmd8, "Eighth");
    list[7] = cmd8;
    tokens_shift_left(list, 4,6);
    int index;
    for(index = 0; list[index] != NULL;index++ ){
        puts(list[index]);
    }
    
    return 0;
}

