#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "codes.h"

void assign_codes( char * codes[], node_t * node, char code[], int n ){

    if( is_leaf( node ) ){
        code[n] = 0;
        codes[node->data] = strdup( code );
    }

    if( node->left ){
        code[n] = '0';
        assign_codes( codes, node->left, code, n+1 );
    }

    if( node->right ){
        code[n] = '1';
        assign_codes( codes, node->right, code, n+1 );
    }

}

