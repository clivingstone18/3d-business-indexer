/*
    This defines prototypes for kd tree-related functions.
*/

#include "data.h"
#define DIMENSIONS 2

struct kd_node;

/* Created a new node to the KD tree */
struct kd_node *create_kd_node(struct dataMapping *mapping, char *row);

/* Inserts a new node into the KD tree */
struct kd_node *insert_to_kd_tree(struct kd_node *kd_tree_head, struct dataMapping *mapping,
                                  char *row, unsigned depth);

/* Searching the nearest node in the tree */
void search_nearest_point(struct kd_node *kd_tree, struct dataMapping *mapping, struct key *key, double *smallest_dist, struct kd_node **smallest_node,

                          struct data ***data, int *numComparisons,

                          unsigned depth, int *foundMatches);

/* Searching for nodes within a defined radius */
void search_within_radius(struct kd_node *kd_tree, struct dataMapping *mapping, struct key *key, double radius, struct data ***data,
                          int *numComparisons, unsigned depth, int *foundMatches);

/* Returns the euclidean distance between two points */
double dist(double a[DIMENSIONS], double b[DIMENSIONS]);

/* Returns the distance between the x or y coordinates two points */

double dist_one(double a, double b);

/* Returns whether a point is within the radius of the other */
int within_radius(double A[DIMENSIONS], double B[DIMENSIONS], double radius);

/* Frees the KD Tree */
void free_kd_tree(struct kd_node *kd_node, struct dataMapping *mapping);
