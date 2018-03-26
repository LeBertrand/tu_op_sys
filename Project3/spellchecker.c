
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