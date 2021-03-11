/*
    Simple utilities.
*/
#include <stdio.h>

/*
    Small wrapper around getline which drops the newline if it
    is present.
*/
ssize_t getlineClean(char **lineptr, size_t *n, FILE *stream);

