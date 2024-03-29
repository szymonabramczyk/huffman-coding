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

int num_bits, byte_val, num_bytes;

void write_bit( FILE * ouf, char bit ){

	byte_val <<= 1;

	if( bit == '1' ) byte_val |= 1;

	num_bits++;

	if( num_bits == 8 ) {
		fputc( byte_val, ouf );
		num_bytes++;
		num_bits = 0;
		byte_val = 0;
	}
}

void encode_tree ( FILE * ouf, node_t * node ){

    if ( is_leaf( node ) ){
        write_bit( ouf, '1');
        char tmp = node->data;
        for (int j = 0; j < 8; j++) {
            write_bit( ouf, !!((tmp << j) & 0x80) + '0');
        }
    }
    else
    {
        write_bit( ouf, '0');
        encode_tree( ouf, node->left );
        encode_tree( ouf, node->right );
    }
}

void encode_file( unsigned char * buffer, int filelen, FILE * ouf, char * codes[] ){

	byte_val = 0;
	num_bits = 0;
	num_bytes = 0;

	for( int i = 0; i < filelen; i++ ){
        char * s = codes[ buffer[i] ];
        while( *s ){
            write_bit ( ouf, *s );
            s++;
        }
	}

	while( num_bits ) 
        write_bit( ouf, '0' );
}


