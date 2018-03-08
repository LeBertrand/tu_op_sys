#include "reboot_framework.c"

int main()
{
    char** tokens = (char**) malloc(4*sizeof(char*));
    char *cmd1 = (char*) malloc(LONGESTWORD),
    *cmd2 = (char*) malloc(LONGESTWORD),
    *cmd3 = (char*) malloc(LONGESTWORD),
    *cmd4 = (char*) malloc(LONGESTWORD);

    strcpy(cmd1, "logger.exe");
    strcpy(cmd2, "&");
    tokens[0] = cmd1;
    tokens[1] = cmd2;
    if(runbg(tokens)){
        puts("Command runs in background.");
    } else {
        puts("No background command.");
    }

    return 0;
}
