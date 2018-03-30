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
    // If no FILE is open, either because NULL was passed or open failed, default.
    if( filename == NULL || wordsfile.dictionary == NULL){
        wordsfile.dictionary = fopen("dictionary", "r");
    }
    // If no dictionary file was loaded, don't terminate process, but print error.
    if(wordsfile.dictionary == NULL)
        fprintf(stderr, "Failed to load dictionary. All spell checks will return false");
    return wordsfile;
}

boolean_t spellcheck_searchword(Wordslist_t *wl, char *target)
{
    rewind(wl->dictionary);
    char buf[30];
    while( !feof(wl->dictionary) ){

        // Read in next word
        fgets(buf, 30, wl->dictionary);
        // Trim newline
        if( buf[ strlen(buf) - 1] == '\n')
            buf[ strlen(buf) - 1] = '\0';

        // Check for match with target.
        if( ! strcmp(buf, target) ) return true;
    } // Finished checking file. @post - target isn't in dictionary.
    return false;
}
