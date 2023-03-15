#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

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

int is_leaf ( node_t * root ){

    return !( root->left ) && !( root->right) ;

}

int is_size_one ( heap_t * h ) {

    return ( h->size == 1);

}

/*combines two node_ts into one
node_t * combine_node_ts( node_t * left, node_t * right ){
    
    node_t * new = ( node_t* ) malloc( sizeof(node_t) );

    new->data = '$';
    new->frequency = left->frequency + right->frequency;
    new->left = left;
    new->right = right;

    return new;

}*/