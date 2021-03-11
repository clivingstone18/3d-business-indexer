/*
    This defines implementations for kd-tree-related functions.
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "data.h"
#include "kd.h"
#include "ll.h"

struct kd_node
{
    struct key *key;
    struct ll *ll;
    struct kd_node *left;
    struct kd_node *right;
    double *point;
};

struct kd_node *create_kd_node(struct dataMapping *mapping, char *row)
{
    struct kd_node *new_kd_node = (struct kd_node *)malloc(sizeof(struct kd_node));
    assert(new_kd_node);
    new_kd_node->ll = NULL;
    new_kd_node->key = NULL;
    new_kd_node->left = NULL;
    new_kd_node->right = NULL;
    new_kd_node->point = NULL;

    new_kd_node->ll = prependToList(new_kd_node->ll, mapping, row);
    getKey(row, mapping, &new_kd_node->key);
    new_kd_node->point = get_coordinates(mapping, new_kd_node->key);

    return new_kd_node;
}

struct kd_node *insert_to_kd_tree(struct kd_node *kd_tree_head, struct dataMapping *mapping,
                                  char *row, unsigned depth)
{
    unsigned cd = depth % DIMENSIONS;
    struct key *tmp_key = NULL;
    getKey(row, mapping, &tmp_key);
    double *curr_point = get_coordinates(mapping, tmp_key);

    if (kd_tree_head == NULL)
    {
        /*Create a new KD node for the data as coordinates do not yet exist in KD tree*/
        struct kd_node *new_kd_node = create_kd_node(mapping, row);
        assert(new_kd_node);
        freeKey(&tmp_key, mapping);
        free(curr_point);
        return new_kd_node;
    }
    /*If the x/y coordinates are equal, check the other coordinate. */
    if (curr_point[cd] == (kd_tree_head->point[cd]))
    {
        unsigned other_cd = (depth + 1) % DIMENSIONS;
        if (curr_point[other_cd] == (kd_tree_head->point[other_cd]))
        {
            /* Add to the head of the linked list */
            kd_tree_head->ll = prependToList(kd_tree_head->ll, mapping, row);
        }
        else
        {
            /* Only one coordinate is equal, so add to the right */
            /*Insert to the right node*/
            kd_tree_head->right = insert_to_kd_tree(kd_tree_head->right, mapping, row, depth + 1);
        }
    }
    else if (curr_point[cd] < (kd_tree_head->point[cd]))
    {
        kd_tree_head->left = insert_to_kd_tree(kd_tree_head->left, mapping, row, depth + 1);
    }
    else
    {
        kd_tree_head->right = insert_to_kd_tree(kd_tree_head->right, mapping, row, depth + 1);
    }

    freeKey(&tmp_key, mapping);
    free(curr_point);

    /*Return the head of the tree*/
    return kd_tree_head;
};

double dist(double a[DIMENSIONS], double b[DIMENSIONS])
{
    double dist = sqrt(((a[0] - b[0]) * (a[0] - b[0])) + ((a[1] - b[1]) * (a[1] - b[1])));
    return (dist);
}

int within_radius(double A[DIMENSIONS], double B[DIMENSIONS], double radius)
{

    if (dist(A, B) <= radius)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

double dist_one(double a, double b)
{
    double dist = fabs(a - b);
    return (dist);
}

void search_nearest_point(struct kd_node *kd_tree, struct dataMapping *mapping, struct key *key, double *smallest_dist, struct kd_node **smallest_node,
                          struct data ***data, int *numComparisons,
                          unsigned depth, int *foundMatches)
{
    struct kd_node *current = kd_tree;
    unsigned cd = depth % DIMENSIONS;
    double *search_point = get_coordinates(mapping, key);

    if (!current)
    {
        free(search_point);
        return;
    }
    /* Increment the number of visits if visited a valid node*/
    (*numComparisons)++;

    if (dist(search_point, current->point) < *smallest_dist)
    {
        /* Updates the smallest distance and node with the smallest distance to the current node */
        *smallest_dist = dist(search_point, current->point);
        *smallest_node = current;
    }

    /* Both sides of the tree have to be checked at this point*/
    if (dist_one(search_point[cd], current->point[cd]) <= *smallest_dist)
    {
        /* Compares the coordinate of interest to determine which side to check first */
        if (search_point[cd] < current->point[cd])
        {
            /* Search the left side first */
            search_nearest_point(current->left, mapping, key, smallest_dist, smallest_node, data, numComparisons, depth + 1, foundMatches);
            search_nearest_point(current->right, mapping, key, smallest_dist, smallest_node, data, numComparisons, depth + 1, foundMatches);
        }
        else
        {
            /* Search the right side first */
            search_nearest_point(current->right, mapping, key, smallest_dist, smallest_node, data, numComparisons, depth + 1, foundMatches);
            search_nearest_point(current->left, mapping, key, smallest_dist, smallest_node, data, numComparisons, depth + 1, foundMatches);
        }

        if (dist(search_point, current->point) <= *smallest_dist)
        {
            /* Writing to results */
            /* Overwriting in case of multiple calls.*/
            *foundMatches = 0;
            struct ll *head = kd_tree->ll;
            write_result_data(head, data, foundMatches);
        }
    }

    /* If the distance is greater than the smallest distance, only one side needs to be checked,
    depending on the result of the key comparison */
    else
    {
        if (search_point[cd] < current->point[cd])
        {
            search_nearest_point(current->left, mapping, key, smallest_dist, smallest_node, data, numComparisons, depth + 1, foundMatches);
        }
        else
        {
            search_nearest_point(current->right, mapping, key, smallest_dist, smallest_node, data, numComparisons, depth + 1, foundMatches);
        }
    }
    free(search_point);
}

void search_within_radius(struct kd_node *kd_tree, struct dataMapping *mapping, struct key *key, double radius, struct data ***data,
                          int *numComparisons, unsigned depth, int *foundMatches)
{
    struct kd_node *current = kd_tree;
    unsigned cd = depth % DIMENSIONS;
    double *search_point = get_coordinates(mapping, key);

    if (!current)
    {
        /* Search has ended */
        free(search_point);
        return;
    }
    /* Increment the number of visits if visited a valid node*/
    (*numComparisons)++;

    if (within_radius(search_point, kd_tree->point, radius))
    {
        /* Writing to results */
        struct ll *head = kd_tree->ll;
        write_result_data(head, data, foundMatches);
    }

    /* Checking the distance between the query and current point on x/y only */
    if (dist_one(search_point[cd], current->point[cd]) <= radius)
    {
        /* Go down both*/
        search_within_radius(kd_tree->left, mapping, key, radius, data, numComparisons, depth + 1, foundMatches);
        search_within_radius(kd_tree->right, mapping, key, radius, data, numComparisons, depth + 1, foundMatches);
    }

    else
    {
        /* Just search the right */
        if (search_point[cd] < current->point[cd])
        {
            search_within_radius(kd_tree->left, mapping, key, radius, data, numComparisons, depth + 1, foundMatches);
        }
        /* Just search the left */
        else
        {
            search_within_radius(kd_tree->right, mapping, key, radius, data, numComparisons, depth + 1, foundMatches);
        }
    }
    free(search_point);
}

void free_kd_tree(struct kd_node *kd_node, struct dataMapping *mapping)
{
    if (!kd_node)
    {
        return;
    }
    free_kd_tree(kd_node->left, mapping);
    free_kd_tree(kd_node->right, mapping);
    freeLL(kd_node->ll, mapping);
    freeKey(&kd_node->key, mapping);
    free(kd_node->point);
    free(kd_node);
    return;
}
