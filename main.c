/*
    A dictionary based on the CLUE database
    Run with
        dict datafile outputfile < inputfile
    
    where 
        datafile is a CLUE csv,
        outputfile is the name of the file which data will be output to,
    
    and
        inputfile is in the format of the business name to search for,
        with one business name per line.
    
    Written by Claire Livingstone for Algorithms and Data Structures, adapted from the 
    provided Assignment 1 Sample Solutions written by Grady  
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "read.h"
#include "dict.h"
#include "utils.h"

int main(int argc, char **argv)
{

    if (argc < 3)
    {
        fprintf(stderr, "Incorrect arguments\n");
        fprintf(stderr, "Run in the form\n");
        fprintf(stderr, "\tdict datafile outputfile < inputfile\n");
        exit(1);
    }

    char *line = NULL;
    size_t size = 0;

    struct dict *dict = readFile(argv[1]);

    struct searchResult *result = NULL;
    FILE *outputFile = fopen(argv[2], "w");
    assert(outputFile);

    int num_comparisons = 0;
    int num_res = 0;

    while (getlineClean(&line, &size, stdin) != (-1))
    {
        result = queryDict(dict, line);
        writeSearchResult(result, dict, outputFile);
        num_res++;
        addComparisons(&num_comparisons, result);
        freeSearchResult(result);
    }

    if (dict)
    {
        freeDict(dict);
    }

    if (line)
    {
        free(line);
    }

    if (outputFile)
    {
        fclose(outputFile);
    }

    return 0;
}
