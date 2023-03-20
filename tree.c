#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "heap.h"

node_t * build_huffman_tree( int freq[], int n_freq )
{
    node_t * left, * right, * father;

    heap_t * heap = create_build_heap( freq, n_freq );
 
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

