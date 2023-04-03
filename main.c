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
    if( out_name == NULL ){     //if out_name is NULL the program generates name automatically
                                //taking input file name and adding extension .huff
        char * name = malloc( strlen( in_name ) * sizeof(char) );
        
        int ind = 0;
        if( ( ext = strrchr( in_name, '.' ) ) == NULL ){        //looks for a dot starting from the end
            printf("WARNING! The input file has no extension!\n");
        }
        else{
            if( strcmp( ext, ".huff" ) == 0 ){
                fprintf( stderr, "%s: input file name \"%s\" has .huff extension\n\n", argv[0], in_name );
                exit( EXIT_FAILURE );
            }
            ind = (int)(ext - in_name);     //see how many characters the name without extension has
            strncpy(name, in_name, ind );
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

    fseek( inf, 0, SEEK_END );      //go to the end of the file
    filelen = ftell( inf );         //check the length of the file
    rewind( inf );                  //go back to the start of the file

    if( filelen == 0 ){
        fprintf( stderr, "%s: the input file is empty!\n\n", argv[0] );
        exit( EXIT_FAILURE );
    }                    
   
    printf( "Output file name: %s\n", out_name );

    //allocate memory and read the file
    buffer = ( unsigned char* )malloc( filelen * sizeof( char ) ); 
    fread( buffer, filelen, 1, inf ); 
    fclose( inf );

    //check frequency of each byte
    for( int i = 0; i < filelen; i++ ){
        freq[ buffer[i] ]++;
    }

    node_t * tree = build_huffman_tree( freq, 256 );

    char * codes[256] = {0};
    int top = 0;
    char code[256];
    node_t * tree_tmp_encode = tree;        //temporary pointers, that we need for different functions
    node_t * tree_tmp_graphic = tree;       //they point to the root of the huffman tree
    node_t * tree_tmp_free = tree;
    assign_codes( codes, tree, code, top );
    
    if( print_table ){
        printf( "byte\tfreq\tcode\n" );
    }

    //check the number of leaves and print table
    int leaves_num = 0;
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

    fputc( 0, ouf );    //write 0 byte that we will use later

    fputc( ( char )( leaves_num - 1 ), ouf ); //write number of leaves byte

    //write extension
    if( ext != NULL){
        fputs( ext, ouf );
    }
    fputc( '/', ouf ); //write the end of extension

    encode_tree( ouf, tree_tmp_encode );    //encode huffman tree and write it to file

    encode_file( buffer, filelen, ouf, codes ); //encode input file and write it to file

    finish_file( ouf ); //finish the file and go back to first byte 
                        //to write the number of irrelevant bits at the end of the file

    print_compression_ratio( filelen );

    //free all the allocated memory
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