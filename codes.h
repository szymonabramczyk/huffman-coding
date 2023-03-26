#ifndef _CODES_H
#define _CODES_H

#include "heap.h"

void assign_codes( char * codes[], node_t * node, char code[], int n );

void write_bit( FILE * ouf, char b );

void encode_tree ( FILE * ouf, node_t * node );

void encode_file( unsigned char * buffer, int filelen, FILE * ouf, char * codes[] );

#endif