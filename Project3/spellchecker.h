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
