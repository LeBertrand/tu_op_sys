#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef BOOLEAN_T
#define BOOLEAN_T
typedef enum { false = 0, true = 1 } boolean_t;
#endif

// File hold file pointer for words list.
FILE *dictionary;

/*
 *  Initialize spellchecker by reading in dictionary.
 *  return - true if dictionary file open for reading, else false
 */
boolean_t initialize();

/*
 *  Read lines of dictionary looking for one that matches argument. Return false
    if end of file reached.
 *  param target - word to search for
 *  return - true if match found, else false
 */
boolean_t searchword(char *target);

// /*
//  *  Main loop
//  */
// boolean_t loop();

int main(int argc, char *argv[])
{
    if( ! initialize() ){
        puts("Can't read dictionary file.");
        return 1;
    }
    
    if(argc > 1)
        printf( "%s%s", argv[1], searchword( argv[1] ) ? " is correct\n" : " is incorrect\n");
    
    fclose(dictionary);
    return 0;
}

boolean_t initialize()
{
    dictionary = fopen("dictionary", "r");
    return dictionary != NULL;
}

boolean_t searchword(char *target)
{
    rewind(dictionary);
    char buf[30];
    while( !feof(dictionary) ){
        // Trim newline
        
        fgets(buf, 30, dictionary);
        strcpy(buf, strtok(buf, " \n"));
        if( ! strcmp(buf, target) ) return true;
    }
    return false;
}