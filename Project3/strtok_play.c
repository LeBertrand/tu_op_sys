#include <stdio.h>
#include <string.h>

int main()
{
    char* tokens[15];
    char str[50];
    strcpy(str, "main.exe -rc force | wc > log.txt");
    const char cuts[3] = " |";
    char* current = strtok(str, cuts);

    int i;
    for( i = 0; i < 15 && current != NULL; i++){
        tokens[i] = (current = strtok(NULL, cuts));
    }
    i--;
    
    puts("Input tokenized. Print tokens.");
    while(i > 0){
        puts(tokens[--i]);
    }

    return 0;
}
