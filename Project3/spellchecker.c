#include "spellchecker.h"
// /*
//  *  Main loop
//  */
// boolean_t loop();

// int main(int argc, char *argv[])
// {
//     if( initialize() == NULL ){
//         puts("Can't read dictionary file.");
//         return 1;
//     }
    
//     if(argc > 1)
//         printf( "%s%s", argv[1], searchword( argv[1] ) ? " is correct\n" : " is incorrect\n");
    
//     fclose(dictionary);
//     return 0;
// }


Wordslist_t spellcheck_initialize(char *filename)
{
    
    
    Wordslist_t wordsfile;
    // Check for filename and try to open it
    if(filename != NULL){
        wordsfile.dictionary = fopen(filename, "r");
    }
    if( filename == NULL || wordsfile.dictionary = NULL){
        wordsfile.dictionary = fopen("dictionary", "r");
    }
    if(Wordsfile.dictionary != NULL) return Wordsfile;
    return NULL;
}

boolean_t spellcheck_searchword(Wordslist_t *wl, char *target)
{
    rewind(wl->dictionary);
    char buf[30];
    while( !feof(wl->dictionary) ){
        // Trim newline
        
        fgets(buf, 30, wl->dictionary);
        strcpy(buf, strtok(buf, " \n"));
        if( ! strcmp(buf, target) ) return true;
    }
    return false;
}