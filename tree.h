#ifndef _TREE_H
#define _TREE_H

#include "heap.h"

node_t * build_huffman_tree( int freq[], int n_freq );

void print_subtree( node_t * root, int indent, int is_left );

void print_tree( node_t * root );

#endif