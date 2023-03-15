#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "heap.h"

//builds a tree
node_t * build_huffman_tree( char data[], int freq[], int size)
{
    node_t * left, * right, * father;

    heap_t * heap = create_build_heap( data, freq, size );
 
    while ( !isSizeOne( heap ) ) {
 
        left = min_node_extract( heap );
        right = min_node_extract( heap );
 
        father = new_node( '$', left->frequency + right->frequency );
 
        father->left = left;
        father->right = right;
 
        insert_node( heap, father );
    }
    
    return min_node_extract( heap );
}

