/*
    This defines implementations for linked-list-related functions.
*/
#include "data.h"
#include <stdlib.h>
#include <assert.h>

#include <stdio.h>
struct ll
{
    struct data *data;
    struct ll *next;
};

/* Create a new empty linked list node. */
struct ll *newNode();

struct ll *newList(char *header)
{
    return NULL;
}

struct key
{
    char **keys;
};

struct data
{
    char **data;
};

struct ll *newNode()
{
    struct ll *returnNode = (struct ll *)malloc(sizeof(struct ll));
    assert(returnNode);
    returnNode->data = NULL;
    returnNode->next = NULL;
    return returnNode;
}

struct ll *prependToList(struct ll *ll, struct dataMapping *mapping, char *row)
{
    struct ll *newHead = newNode();
    getData(row, mapping, &(newHead->data));
    newHead->next = ll;
    return newHead;
}

void freeLL(struct ll *ll, struct dataMapping *mapping)
{
    if (!ll)
    {
        return;
    }
    struct ll *current = ll;
    struct ll *next;
    while (current)
    {
        next = current->next;
        freeData(&current->data, mapping);
        free(current);
        current = next;
    }
}

void write_result_data(struct ll *head, struct data ***data, int *foundMatches)
{
    int i = 0;
    while (head)
    {
        *data = realloc(*data, sizeof(struct data *) * (*foundMatches + 2));
        assert(*data);
        (*data)[*foundMatches] = head->data;
        (*data)[*foundMatches + 1] = NULL;

        (*foundMatches)++;
        i++;
        head = head->next;
    }
}