#include <stdio.h>
#include <stdlib.h>

void usage()
{
    fprintf(stderr, "usage info\n----------\n");
    fprintf(stderr, "stdin->stdout: ./ascii-only --\n");
    fprintf(stderr, "stdin->outfile: ./ascii-only --outfile\n");
    fprintf(stderr, "infile->stdout: ./ascii-only infile\n");
    fprintf(stderr, "infile->outfile: ./ascii-only infile outfile\n");
    exit(0);
}

void arg_check(char **argv) {

    char file_check[2];
    int i;

    file_check[0] = (char)argv[1][0];
    file_check[1] = (char)argv[1][1];

    if(file_check[0] == '-' && file_check[1] == '-' && argv[1][2] == 0) {
        /* stdin -> stdout */
        while (EOF != fscanf(stdin,"%c",(char *)&i)) {
            if (!(i&128)) {
                printf("%c",(char)(127&i));
            }
        }
    }
    else if(file_check[0] == '-' && file_check[1] == '-' && argv[1][2] != 0 && argv[2] == NULL) {
        /* stdin -> outfile */
        FILE *outfile;
        char *outname;
        int i;
        int outlength = 0;

        for(i = 2; ; i++) {
            if(argv[1][i] == 0) {
                break;
            }
            outlength++;
        }
        outlength = 0;
        outname = (char *)malloc(sizeof(char) * (outlength + 1));
        for(i = 2; ; i++) {
            outname[outlength++] = argv[1][i];
            if(argv[1][i] == 0) {
                break;
            }
        }
        outfile = fopen(outname, "w");
        if(!outfile) {
            perror(outname);
            exit(0);
        }
        while (EOF != fscanf(stdin,"%c",(char *)&i)) {
            if (!(i&128)) {
                fprintf(outfile, "%c",(char)(127&i));
            }
        }
        fclose(outfile);
        free(outname);
    }
    else if(file_check[0] != '-' && file_check[1] != '-' && argv[1][2] != 0 && argv[2] == NULL) {
        /* infile -> stdout */
        FILE *infile;
        infile = fopen(argv[1], "r");

        if(!infile) {
            perror(argv[1]);
            exit(0);
        }
        while (EOF != fscanf(infile,"%c",(char *)&i)) {
            if (!(i&128)) {
                fprintf(stdout, "%c",(char)(127&i));
            }
        }
    } 
    else if(argv[1] != NULL && argv[2] != NULL && file_check[0] != '-' && file_check[1] != '-') {
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

        while (EOF != fscanf(infile,"%c",(char *)&i)) {
            if (!(i&128)) {
                fprintf(outfile, "%c",(char)(127&i));
            }
        }
        fclose(infile);
        fclose(outfile);
    } else {
        usage();
    }
}


int main(int argc, char **argv)
{
    // int i;
    
    if (argc < 2 || argc > 3){
        usage();
    }


    arg_check(argv);



    return 0;
}
