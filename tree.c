#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tree.h"
#include "heap.h"

node_t * build_huffman_tree( int freq[], int n_freq )
{
    node_t * left, * right, * father;

    heap_t * heap = create_build_heap( freq, n_freq );
 
    while ( !is_size_one( heap ) ) {
 
        left = min_node_extract( heap );
        right = min_node_extract( heap );
 
        father = new_node( '$', left->frequency + right->frequency );
 
        father->left = left;
        father->right = right;
 
        insert_node( heap, father );
    }
    
    return min_node_extract( heap );
}

void print_subtree( node_t * root, int indent, int is_right ) {
    bool * right = malloc( 256 * sizeof( bool ) );
    for( int i = 0; i < 256; i++ )
        right[i] = 0;
    
    right[indent] = is_right;

    if( root == NULL )
        return;

    int i;
    //all levels until (without) level above current level
    for ( i = 0; i < indent; i++ )
    {
        printf( "%s", right[i] ? "   " : "|  " );
    }

    //level directly above current level
    if ( i <= indent )
    {
        printf( "%s", right[i] ? "└─" : "├─" ) ;
    }
    
    //current level
    if ( right[i] ){
        printf( "(1)%d\n", root->data );
    } else {
        printf( "(0)%d\n", root->data );
    }

    //child levels
    print_subtree( root->left, indent + 1, 0 );
    print_subtree( root->right, indent + 1, 1 );
    
}

void print_tree( node_t * root ) {
    if( root != NULL ) {
        printf( "%d\n", root->data );
        print_subtree( root->left,  0, 0 );
        print_subtree( root->right, 0, 1 );
    }
}
