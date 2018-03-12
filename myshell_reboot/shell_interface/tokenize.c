#include "shell_interface.h"

void tokenize(char** tokens, char* linein)
{
    // String pointer for individual tokens in linein
    char* current = strtok(linein, " ");
    
    // Index to iterate over tokens array
    int tokensread;
    
    for(tokensread = 0; current != NULL; tokensread++){
        tokens[tokensread] = (char*) malloc(LONGESTWORD);
        strcpy(tokens[tokensread], current);
        current = strtok(NULL, " ");
    }
    // Null terminate
    tokens[tokensread] = NULL;
}