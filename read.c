/*
    This defines implementations for read-related functions.
*/

#include "dict.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "utils.h"

struct dict *readFile(char *filename)
{
    FILE *file = NULL;
    struct dict *returnDict = NULL;
    file = fopen(filename, "r");
    assert(file);

    char *line = NULL;
    size_t size = 0;

    /* First line is headers, so we handle differently. */
    if (getlineClean(&line, &size, file) != (-1))
    {
        returnDict = newDict(line);
    }

    /* Rest of lines are rows. */
    while (getlineClean(&line, &size, file) != (-1))
    {
        addDictEntry(returnDict, line);
    }

    if (line)
    {
        free(line);
    }
    if (file)
    {
        fclose(file);
    }
    return returnDict;
}
