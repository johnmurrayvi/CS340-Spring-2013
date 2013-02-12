#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

void asciirw(FILE *in, FILE *out)
{
    unsigned char c;
    while (!feof(in)) {
        c = (unsigned char) fgetc(in);
        if (c > 0 && c < 128)
            printf("%c", (int) c);
        else if (c >= 128 && !feof(in)) {
            /* EOF = -1  -->  255 for unsigned char */
            fprintf(stderr, "Non-ascii character detected\n");
            return;
        }
    }
}

int main(int argc, char **argv)
{
    FILE *f;

    if (argc < 2){
        fprintf(stderr, "usage: ./ascii-only filename\n");
        exit(1);
    }

    f = fopen(argv[1],"r");

    if (!f) {
        perror("error from fopen: ");
        errno;
        return 1;
    }

    fclose(f);
    return 0;
}

