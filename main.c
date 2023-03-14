#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

char * usage =
  "Usage: %s [options] -i input_file -o output_file \n"
  "     List of options:\n"
  "         * -a - print additional info into stdout; \n";

int
main (int argc, char **argv)
{
    int opt;
    char * in_name = NULL;
    char * out_name = NULL;
    int print_info = 0;
    char * prog_name = argv[0];

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
}