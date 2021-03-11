/*
    This defines prototypes for data-related functions.
*/

struct data;

struct key;

struct dataMapping;

/*
    Read the given line and store the mapping from row to values.
*/
struct dataMapping *getDataMapping(char *header);

/*
    Read a line into the given data locations.
*/
void getData(char *row, struct dataMapping *mapping,
             struct data **data);

/*
    Read a line into the given key locations.
*/

void getKey(char *row, struct dataMapping *mapping, struct key **key);

/*
    Converts coordinates as the key into array of doubles
*/

double *get_coordinates(struct dataMapping *mapping, struct key *key);
/*
    Read a line as a key and return it.
*/
struct key *readKey(char *row, struct dataMapping *mapping, char **radius);

/* 
    Frees the data mapping 
*/
void freeDataMapping(struct dataMapping **mapping);
/*
    Frees the key and data in the given pair.
*/
void freeKeyPair(struct data **dataLoc, struct key **keyLoc, struct dataMapping *mapping);

/*
    Frees a standalone key.
*/
void freeKey(struct key **keyLoc, struct dataMapping *mapping);

/*
    Frees an array of data
*/
void freeData(struct data **dataLoc, struct dataMapping *mapping);

/*
    Allocates space for a string representing the given
    key and returns it.
*/
char *getKeyString(struct dataMapping *mapping, struct key *key);

/*
    Allocates space for a string representing the given
    data and returns it.
*/
char *getDataString(struct dataMapping *mapping, struct data *data);
