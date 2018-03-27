#include "spellchecker.h"

int main()
{
    Wordslist_t wl = spellcheck_initialize(NULL);
    printf( "%s%s", "Apollo", spellcheck_searchword( &wl, "Apollo" ) ? " is correct\n" : " is incorrect\n");
    printf( "%s%s", "Argv", spellcheck_searchword( &wl, "Argv" ) ? " is correct\n" : " is incorrect\n");
    
    return 0;
}