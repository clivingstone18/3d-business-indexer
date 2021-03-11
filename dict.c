/*
    This defines implementations for dictionary-related functions.
*/

#include "dict.h"
#include "data.h"
#include "kd.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define KEYDATAJOIN "-->"
#define FAILTEXT "NOTFOUND"
#define DEPTH 0

struct dict
{
    struct dataMapping *mapping;
    struct kd_node *kd_tree;
};

struct key
{
    char **keys;
};

struct data
{
    char **data;
};

struct kd_node
{
    struct key *key;
    struct ll *ll;
    double *point;
    struct kd_node *left;
    struct kd_node *right;
};

struct ll
{
    struct data *data;
    struct ll *next;
};

struct searchResult
{
    struct key *key;
    char *radius;
    struct dataMapping *mapping;
    struct data **data;
    int numComparisons;
};

struct dict *newDict(char *header)
{
    struct dict *returnDict = (struct dict *)malloc(sizeof(struct dict));
    assert(returnDict);
    returnDict->mapping = getDataMapping(header);
    returnDict->kd_tree = NULL;
    return returnDict;
}

void addDictEntry(struct dict *dict, char *row)
{
    dict->kd_tree = insert_to_kd_tree(dict->kd_tree, dict->mapping, row, DEPTH);
}

struct searchResult *queryDict(struct dict *dict, char *string)
{
    char **radius = malloc(sizeof(char *));
    assert(radius);
    *radius = NULL;

    struct key *searchKey = readKey(string, dict->mapping, radius);
    struct searchResult *result = (struct searchResult *)
        malloc(sizeof(struct searchResult));
    assert(result);
    result->key = searchKey;
    result->radius = *radius;
    result->numComparisons = 0;
    result->data = (struct data **)malloc(sizeof(struct data *));
    assert(result->data);
    *(result->data) = NULL;

    result->mapping = dict->mapping;

    double dummy = 100000;
    int i = 0;

    if (*radius != NULL)
    {
        search_within_radius(dict->kd_tree, dict->mapping, searchKey, atof(*radius), &(result->data), &result->numComparisons, DEPTH, &i);
    }
    else
    {
        struct kd_node **smallest_node = malloc(sizeof(struct kd_node *));
        assert(smallest_node);
        *smallest_node = dict->kd_tree;
        search_nearest_point(dict->kd_tree, dict->mapping, searchKey, &dummy, smallest_node, &(result->data), &(result->numComparisons), DEPTH, &i);
        free(smallest_node);
    }
    free(radius);
    return result;
}

void addComparisons(int *num_comparisons, struct searchResult *result)
{
    int i = 0;
    if (!result->data[i])
    {
        result->numComparisons = 0;
    }
    *num_comparisons = *num_comparisons + result->numComparisons;
}

void writeSearchResult(struct searchResult *result, struct dict *dict, FILE *file)
{
    char *keyText;
    char *resultText;
    int i;
    /* Commented out code is fairly clean way output-wise to display results */
    // int j;
    keyText = getKeyString(dict->mapping, result->key);
    i = 0;

    if (!result->data[i])
    {
        resultText = FAILTEXT;
        result->numComparisons = 0;
        if (result->radius)
        {
            fprintf(file, "%s %s %s %s\n", keyText, result->radius, KEYDATAJOIN, resultText);
        }
        else
        {
            fprintf(file, "%s %s %s\n", keyText, KEYDATAJOIN, resultText);
        }
    }

    while ((result->data)[i])
    {
        // if(! first){
        //     for(j = 0; j < indentLength; j++){
        //         fprintf(file, " ");
        //     }
        // }
        fprintf(file, "%s", keyText);
        resultText = getDataString(dict->mapping, (result->data)[i]);
        fprintf(file, " %s %s\n", KEYDATAJOIN, resultText);
        if (resultText)
        {
            free(resultText);
        }
        i++;
    }

    if (result->numComparisons == 0)
    {
        printf("%s %s %s %s\n", keyText, result->radius, KEYDATAJOIN, FAILTEXT);
    }
    else if (result->radius)
    {
        printf("%s %s %s %d\n", keyText, result->radius, KEYDATAJOIN, result->numComparisons);
    }
    else if (!result->radius)
    {
        printf("%s %s %d\n", keyText, KEYDATAJOIN, result->numComparisons);
    }
    if (keyText)
    {
        free(keyText);
    }
}

void freeDict(struct dict *dict)
{
    free_kd_tree(dict->kd_tree, dict->mapping);
    freeDataMapping(&(dict->mapping));
    free(dict);
}

void freeSearchResult(struct searchResult *result)
{
    if (result)
    {
        if (result->key)
        {
            free((result->key->keys)[0]);
            free(result->key->keys);
            free(result->key);
        }
        if (result->data)
        {
            free(result->data);
        }
        free(result);
    }
}
