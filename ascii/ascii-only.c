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

    FILE *infile, *outfile;
    char file_check[2];

    file_check[0] = (char)argv[1][0];
    file_check[1] = (char)argv[1][1];

    if (file_check[0] == '-' && file_check[1] == '-' && argv[2] == NULL) {
        if (argv[1][2] == '\0') {
            /* stdin -> stdout */

            asciirw(stdin, stdout);
        } else if (argv[1][2] != '\0') {
            /* stdin -> outfile */

            char *outname;
            /* Should get argv[1][2]? */
            outname = strdup(((char *) argv[1]) + 2);
            outfile = fopen(outname, "w");
            if (!outfile) {
                perror(outname);
                exit(0);
            }

            asciirw(stdin, outfile);

            fclose(outfile);
            free(outname);
        }
    } else if (file_check[0] != '-' && file_check[1] != '-') {
        infile = fopen(argv[1], "r");
        if (!infile) {
            perror(argv[1]);
            exit(0);
        }

        if (argv[1][2] != '\0' && argv[2] == NULL) {
            /* infile -> stdout */

            asciirw(infile, stdout);
        } else if (argv[2] != NULL) {
            /* infile -> outfile */

            outfile = fopen(argv[2], "w");
            if (!outfile) {
                perror(argv[2]);
                exit(0);
            }

            asciirw(infile, outfile);

            fclose(outfile);
        } else {
            usage();
        }

        fclose(infile);
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
