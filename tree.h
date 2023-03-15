#ifndef _TREE_H
#define _TREE_H


typedef struct e {

    char data;
    long frequency;
    struct e *father, *left, *right;

} node_t;

typedef struct h {

    long size;
    long capacity;
    node_t ** array;

} heap_t;

node_t * new_node( char data, long frequency );

heap_t * create_minheap( long capacity );

void swap_nodes( node_t ** a, node_t ** b);

void min_heapify( heap_t * h, int index );

int is_leaf ( node_t * root );;

int is_size_one ( heap_t * h );

//node_t * combine_nodes( node_t * left, node_t * right );




#endif
