#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "heap.h"
#include "tree.h"
#include "codes.h"



char * usage =
  "Usage: %s [options] -i input_file -o output_file \n"
  "     List of options:\n"
  "         * -a - print additional info into stdout; \n\n";

int
main (int argc, char **argv)
{
    int opt;
    char * in_name = NULL;
    char * out_name = NULL;
    int print_info = 0;
    char * prog_name = argv[0];

    unsigned char * buffer;
    long filelen;

    int freq[256] = {0};

    while ( ( opt = getopt( argc, argv, "i:o:a" ) ) != -1 ) {
        switch( opt ) {
        case 'i':
            in_name = optarg;
            break;
        case 'o':
            out_name = optarg;
            break;
        case 'a':
            print_info = 1;
            break;
        default:                   /* '?' */
            fprintf( stderr, usage, prog_name );
            exit( EXIT_FAILURE );
        }
    }
	if( optind < argc ) {
		fprintf( stderr, "\nBad parameters!\n" );
		for( ; optind < argc; optind++ )
			fprintf( stderr, "\t\"%s\"\n", argv[optind] );
		fprintf( stderr, "\n" );
		fprintf( stderr, usage, prog_name );
		exit( EXIT_FAILURE );
	}
    
    if( in_name == NULL ){
        fprintf( stderr, "\n%s: input file name not given! please specify the name! \n\n", argv[0] );
        fprintf( stderr, usage, prog_name );
        exit( EXIT_FAILURE );
    }

    if( out_name == NULL ){
        fprintf( stderr, "\n%s: output file name not given! please specify the name! \n\n", argv[0] );
        fprintf( stderr, usage, prog_name );
        exit( EXIT_FAILURE );
    }

    FILE * inf = fopen( in_name, "rb" );
    if( inf == NULL ) {
      fprintf( stderr, "%s: can not read input file: %s\n\n", argv[0], in_name );
      exit( EXIT_FAILURE );
    }

    fseek( inf, 0, SEEK_END );          
    filelen = ftell( inf );             
    rewind( inf );                      

    buffer = (unsigned char*)malloc( filelen * sizeof(char) ); 
    fread( buffer, filelen, 1, inf ); 
    fclose( inf );

    for( int i = 0; i < filelen; i++ )
        printf( "%d%d ", i, buffer[i] );
    printf("\n");

    for( int i = 0; i < filelen; i++ ){
        freq[ buffer[i] ]++;
    }

    node_t * tree = build_huffman_tree( freq, 256 );

    char * codes[256] = {0};
    int top = 0;
    char code[] = "";
    assign_codes( codes, tree, code, top );
    
    for( int i = 0; i < 256; i++ ){
        if( freq[i] != 0 )
            printf( "%d: \t %d %s\n", i, freq[i], codes[i]);
    }

    FILE * ouf = fopen( out_name, "wb" );
    if( ouf == NULL ) {
      fprintf( stderr, "%s: can not write output file: %s\n\n", argv[0], out_name );
      exit( EXIT_FAILURE );
    }

    encode_file( buffer, filelen, ouf, codes );

    
    fclose( ouf );
}