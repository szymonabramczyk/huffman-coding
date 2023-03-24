#include <stdio.h>
#include <stdlib.h>
#include "heap.h"

node_t * new_node( unsigned char data, long frequency ) {

    node_t * temp = ( node_t* ) malloc( sizeof(node_t) );

    temp->left = temp->right = NULL;
    temp->data = data;
    temp->frequency = frequency;

    return temp;

}

heap_t * create_minheap( int capacity ){

    heap_t * heap = ( heap_t * ) malloc( sizeof( heap_t ) );

    heap->size = 0;
    heap->capacity = capacity;
    heap->array = ( node_t ** ) malloc( heap->capacity * sizeof( node_t * ) );

    return heap;
}

void swap_nodes( node_t ** a, node_t ** b ){

    node_t * temp = * a;
    * a = * b;
    * b = temp;
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

    return !( root->left ) && !( root->right ) ;

}

int is_size_one ( heap_t * h ) {

    return ( h->size == 1 );

}

node_t * min_node_extract ( heap_t * heap ){

    node_t * temp = heap->array[0];
    heap->array[0] = heap->array[heap->size - 1];
 
    --heap->size;
    min_heapify( heap, 0 ) ;
 
    return temp;

}

void insert_node( heap_t * heap, node_t * node ){
 
    ++heap->size;
    int i = heap->size - 1;
 
    while ( i && ( node->frequency < heap->array[( i - 1 ) / 2]->frequency ) ) {
 
        heap->array[i] = heap->array[( i - 1 ) / 2];
        i = ( i - 1 ) / 2;
    }
 
    heap->array[i] = node;

}

void build_heap( heap_t * heap ){
 
    int n = heap->size - 1;
    int i;
 
    for ( i = ( n - 1 ) / 2; i >= 0; --i )
        min_heapify( heap, i );
}

heap_t * create_build_heap( int freq[], int n_freq ){
    
    int capacity = 0;

    for ( int i = 0; i < n_freq; i++ ){
        if ( freq[i] != 0 )
            capacity++;
    }
    
    heap_t * heap = create_minheap( capacity );
 
    int j = 0;
    for ( int i = 0; i < n_freq; i++ ){

        if( freq[i] != 0 )
            heap->array[j++] = new_node( i, freq[i] );
    }
 
    heap->size = capacity;
    build_heap( heap );
 
    return heap;
}