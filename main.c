#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "heap.h"
#include "tree.h"
#include "codes.h"

char * usage =
  "Usage: %s [options] -i input_file [-o output_file] \n"
  "     List of options:\n"
  "         * -f - print frequencies table into stdout;\n"
  "         * -t - print huffman tree into stdout;\n\n";

int
main ( int argc, char **argv )
{
    int opt;
    char * in_name = NULL;
    char * out_name = NULL;
    bool out_name_given = 0;
    bool print_table = 0;
    bool print_graphic_tree = 0;
    char * prog_name = argv[0];

    unsigned char * buffer;
    long filelen;

    int freq[256] = {0};

    while ( ( opt = getopt( argc, argv, "i:o:ft" ) ) != -1 ) {
        switch( opt ) {
        case 'i':
            in_name = optarg;
            break;
        case 'o':
            out_name = optarg;
            out_name_given = 1;
            break;
        case 'f':
            print_table = 1;
            break;
        case 't':
            print_graphic_tree = 1;
            break;
        default:                   
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

    FILE * inf = fopen( in_name, "rb" );
    if( inf == NULL ) {
      fprintf( stderr, "%s: can not read input file: %s\n\n", argv[0], in_name );
      exit( EXIT_FAILURE );
    }

    char * ext = NULL;
    if( out_name == NULL ){
        char * temp;
        char * name = malloc( strlen( in_name ) * sizeof(char) );
        
        int ind = 0;
        if( ( ext = strrchr( in_name, '.' ) ) == NULL ){
            printf("WARNING! The input file has no extension!\n");
        }
        else{
            if( strcmp( ext, "huff" ) == 0 ){
                fprintf( stderr, "%s: input file name \"%s\" has .huff extension\n\n", argv[0], in_name );
                exit( EXIT_FAILURE );
            }
            ext++;
            ind = (int)(ext - in_name);
            strncpy(name, in_name, ind - 1);
        }

        if( (out_name = malloc( ( strlen( ind != 0 ? name : in_name ) + 6 ) * sizeof(char) ) ) == NULL ){
            fprintf( stderr, "%s: failed to allocate memory for output file name\n\n", argv[0] );
            exit( EXIT_FAILURE );
        }
        if( sprintf( out_name, "%s.huff", ind != 0 ? name : in_name ) == 0 ){
            fprintf( stderr, "%s: failed to save output file name\n\n", argv[0] );
            exit( EXIT_FAILURE );
        }

        free( name );

    }

    fseek( inf, 0, SEEK_END );          
    filelen = ftell( inf );             
    rewind( inf );  

    if( filelen == 0 ){
        fprintf( stderr, "%s: the input file is empty!\n\n", argv[0] );
        exit( EXIT_FAILURE );
    }                    
   
    printf( "Output file name: %s\n", out_name );

    buffer = ( unsigned char* )malloc( filelen * sizeof( char ) ); 
    fread( buffer, filelen, 1, inf ); 
    fclose( inf );

    for( int i = 0; i < filelen; i++ ){
        freq[ buffer[i] ]++;
    }

    node_t * tree = build_huffman_tree( freq, 256 );

    char * codes[256] = {0};
    int top = 0;
    char code[256];
    node_t * tree_tmp_encode = tree;
    node_t * tree_tmp_graphic = tree;
    node_t * tree_tmp_free = tree;
    assign_codes( codes, tree, code, top );
    
    int leaves_num = 0;

    if( print_table ){
        printf( "byte\tfreq\tcode\n" );
    }
    for( int i = 0; i < 256; i++ ){
        if( freq[i] != 0 ){
            leaves_num++;
            if( print_table ) printf( "%d: \t %d\t%s\n", i, freq[i], codes[i] );
        }
    }

    if( print_graphic_tree ){
        print_tree( tree_tmp_graphic );
    }

    FILE * ouf = fopen( out_name, "wb" );
    if( ouf == NULL ) {
      fprintf( stderr, "%s: can not write output file: %s\n\n", argv[0], out_name );
      exit( EXIT_FAILURE );
    }

    fputc( 0, ouf );

    fputc( ( char )( leaves_num - 1 ), ouf );

    if( ext != NULL){
        fputs( ext, ouf );
    }
    fputc( '/', ouf );

    encode_tree( ouf, tree_tmp_encode );

    encode_file( buffer, filelen, ouf, codes );

    finish_file( ouf );

    print_compression_ratio( filelen );

    if( !out_name_given ){
        free( out_name );
    }
    free( buffer );

    free_tree( tree_tmp_free );

    for( int i = 0; i < 256; i++ ){
        if( freq[i] != 0 )
            free( codes[i] );
    }
    
    fclose( ouf );
}