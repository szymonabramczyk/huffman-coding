#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tree.h"
#include "heap.h"

#define MAX_LEVELS 256

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
    node_t * tmp = min_node_extract( heap );
    free_heap( heap );
    return tmp;
}

void print_subtree( node_t * root, int indent, int is_left ) {

    static bool left[MAX_LEVELS] = {0};
    left[indent] = is_left;

    if( root == NULL )
        return;

    int i;
    //all levels until (without) level above current level
    for( i = 0; i < indent; i++ ){
        printf( "%s", left[i] ? "   " : "|  " );
    }

    //level directly above current level
    if( i <= indent ){
        printf( "%s", left[i] ? "└─" : "├─" ) ;
    }
    
    //current level
    if( left[i] ){
        printf( "(0)" );
        if( is_leaf( root ) )
            printf( "%d", root->data );
        printf( "\n" );
    } else {
        printf( "(1)" );
        if( is_leaf( root ) )
            printf( "%d", root->data );
        printf( "\n" );
    }

    //child levels
    print_subtree( root->right, indent + 1, 0 );
    print_subtree( root->left, indent + 1, 1 );
    
}

void print_tree( node_t * root ) {
    if( root != NULL ) {
        printf( "Huffman tree:\n*\n");
        print_subtree( root->right,  0, 0 );
        print_subtree( root->left, 0, 1 );
    }
}

void free_tree( node_t * root ){
    if( root->left )
        free_tree( root->left );
    if( root->right )
        free_tree( root->right );
    free( root );
}
