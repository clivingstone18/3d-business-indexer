/*
    This defines prototypes for read-related functions.
*/

#include "dict.h"

/* 
    Reads a given filename and returns a linked list representing
    the rows in that file.
*/
struct dict *readFile(char *filename);
