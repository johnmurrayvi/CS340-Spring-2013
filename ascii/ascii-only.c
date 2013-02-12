#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void usage()
{
    fprintf(stderr, "usage info\n----------\n");
    fprintf(stderr, "stdin->stdout: ./ascii-only --\n");
    fprintf(stderr, "stdin->outfile: ./ascii-only --outfile\n");
    fprintf(stderr, "infile->stdout: ./ascii-only infile\n");
    fprintf(stderr, "infile->outfile: ./ascii-only infile outfile\n");
    exit(0);
}

void asciirw(FILE *in, FILE *out)
{
    int i;
    while (EOF != fscanf(in,"%c",(char *)&i)) {
        if (!(i&128)) {
            fprintf(out,"%c",(char)(127&i));
        }
    }
}

void arg_check(char **argv) {

    char file_check[2];

    file_check[0] = (char)argv[1][0];
    file_check[1] = (char)argv[1][1];

    if(file_check[0] == '-' && file_check[1] == '-' && argv[1][2] == 0) {
        /* stdin -> stdout */
        asciirw(stdin, stdout);
    } else if(file_check[0] == '-' && file_check[1] == '-' && argv[1][2] != 0 && argv[2] == NULL) {
        /* stdin -> outfile */
        FILE *outfile;
        char *outname, *outarg;

        outarg = ((char *) argv[1]) + 2;    /* Should get argv[1][2]? */
        outname = strdup(outarg);           /* Get name to null ternimation */

        outfile = fopen(outname, "w");
        if(!outfile) {
            perror(outname);
            exit(0);
        }
        asciirw(stdin, outfile);

        fclose(outfile);
        free(outname);
    } else if(file_check[0] != '-' && file_check[1] != '-' && argv[1][2] != 0 && argv[2] == NULL) {
        /* infile -> stdout */
        FILE *infile;
        infile = fopen(argv[1], "r");

        if(!infile) {
            perror(argv[1]);
            exit(0);
        }
        asciirw(infile, stdout);
    } else if(file_check[0] != '-' && file_check[1] != '-' && argv[1] != NULL && argv[2] != NULL) {
        /* infile -> outfile */
        FILE *infile, *outfile;

        infile = fopen(argv[1], "r");
        if(!infile) {
            perror(argv[1]);
            exit(0);
        }
        outfile = fopen(argv[2], "w");
        if(!outfile) {
            perror(argv[2]);
            exit(0);
        }
        asciirw(infile, outfile);

        fclose(infile);
        fclose(outfile);
    } else {
        usage();
    }
}

int main(int argc, char **argv)
{

    if (argc < 2 || argc > 3){
        usage();
    }

    arg_check(argv);

    return 0;
}
