#ifndef _HEAP_H
#define _HEAP_H

typedef struct e {

    unsigned char data;
    int frequency;
    struct e * left, * right;

} node_t;

typedef struct h {

    int size;
    int capacity;
    node_t ** array;

} heap_t;

node_t * new_node( unsigned char data, long frequency );

heap_t * create_minheap( int capacity );

void swap_nodes( node_t ** a, node_t ** b);

void min_heapify( heap_t * h, int index );

int is_leaf( node_t * root );

int is_size_one( heap_t * h );

node_t * min_node_extract( heap_t * heap );

void insert_node( heap_t * heap, node_t * node );

void build_heap( heap_t * heap );

heap_t * create_build_heap( int freq[], int n_freq );

void free_heap( heap_t * h );

#endif