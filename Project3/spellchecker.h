#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef BOOLEAN_T
#define BOOLEAN_T
typedef enum { false = 0, true = 1 } boolean_t;
#endif

/*
 *  Wrapper structure for shared file pointer. Exists only to allow calling
 *  processes to initialize variables without this code living in any process.
 */
typedef struct {
    // File hold file pointer for words list.
    FILE *dictionary;
} Wordslist_t;

Wordslist_t currentData;

/*
 *  Initialize spellchecker by reading in dictionary. If valid file given in
    argument, read file in as dictionary. Put FILE struct in wrapper Wordslist_t
    struct if file read and return struct. Else return NULL.
 *  param filename - String containing filename to use as dictionary
 *  return - struct containing dictionary file if file open for reading, else NULL
 */
Wordslist_t spellcheck_initialize(char *filename);

/*
 *  Read lines of dictionary looking for one that matches argument. 
 *  param target - word to search for
 *  param wl - struct containing dictionary file struct
 *  return - true if match found, else false
 */
boolean_t spellcheck_searchword(Wordslist_t wl, char *target);

