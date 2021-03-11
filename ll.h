/*
    This defines prototypes for linked-list-related functions.
*/

#include "data.h"

struct ll;

/* 
    Creates an empty linked list.
*/
struct ll *newList();

/*
    Prepend the item in the line to the given linked list.
*/
struct ll *prependToList(struct ll *ll, struct dataMapping *mapping, char *row);

/*
    Search for the key in the given list and put the data in the given location.
*/
void searchLL(struct ll *ll, struct dataMapping *mapping, struct key *key,
              struct data ***data);

/*
    Frees all the memory in the given linked list.
*/
void freeLL(struct ll *ll, struct dataMapping *mapping);

/* 
    Writes the data contained in a linked list to a data struct if it meets the criteria
    (smallest distance or within radius). 
*/

void write_result_data(struct ll *head, struct data ***data, int *foundMatches);
