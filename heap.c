#include <stdio.h>
#include <stdlib.h>
#include "heap.h"

//creates a new node_t
node_t * new_node_t( char data, long frequency ) {

    node_t * temp = ( node_t* ) malloc( sizeof(node_t) );

    temp->left = temp->right = NULL;
    temp->data = data;
    temp->frequency = frequency;

    return temp;

}

//creates a heap
heap_t * create_minheap( long capacity ){

    heap_t * heap = ( heap_t* ) malloc( sizeof( heap_t ) );

    heap->size = 0;
    heap->capacity = capacity;
    heap->array = ( node_t** ) malloc( heap->capacity * sizeof(node_t*));

    return heap;
}

//swaps two nodes
void swap_nodes( node_t ** a, node_t ** b){

    node_t ** temp = * a;
    * a = * b;
    * b = * temp;
}

void min_heapify( heap_t * h, int index ){

    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if ( left < h->size && h->array[left]->frequency < h->array[smallest]->frequency )
        smallest = left;

    if ( right < h->size && h->array[right]->frequency < h->array[smallest]->frequency )
        smallest = right;

    if( smallest != index ){
        swap_nodes( &h->array[smallest], &h->array[index] );
        min_heapify( h, smallest );
    }

}

//checks if the node is a leaf
int is_leaf ( node_t * root ){

    return !( root->left ) && !( root->right ) ;

}

//checks if the heap is size 1
int is_size_one ( heap_t * h ) {

    return ( h->size == 1 );

}
//extracts a node with minimal value
node_t * min_node_extract ( heap_t * heap ){

    node_t * temp = heap->array[0];
    heap->array[0] = heap->array[heap->size - 1];
 
    --heap->size;
    min_heapify( heap, 0 ) ;
 
    return temp;

}
//inserts a new node to heap
void insert_node( heap_t * heap, node_t * node ){
 
    ++heap->size;
    int i = heap->size - 1;
 
    while ( i && ( node->frequency < heap->array[( i - 1 ) / 2]->frequency ) ) {
 
        heap->array[i] = heap->array[( i - 1 ) / 2];
        i = ( i - 1 ) / 2;
    }
 
    heap->array[i] = node;

}
//builds heap
void build_heap( heap_t * heap ){
 
    int n = heap->size - 1;
    int i;
 
    for ( i = ( n - 1 ) / 2; i >= 0; --i )
        min_heapify( heap, i );
}
//creates a heap with capacity of size and inserts all data to heap
heap_t * create_build_heap( char data[], int freq[], int size ){
 
    heap_t * heap = create_minheap(size);
 
    for ( int i = 0; i < size; ++i) 
        heap->array[i] = new_node(data[i], freq[i]);
 
    heap->size = size;
    build_heap( heap );
 
    return heap;
}