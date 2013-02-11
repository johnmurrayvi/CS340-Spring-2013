#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char **argv)
{
    unsigned char c;
    FILE *f;
	
    if (argc < 2) {
		fprintf(stderr, "usage: ./ascii-only filename\n");
		exit(1);
	}
	
	f = fopen(argv[1],"r");

    if (!f) {
        perror("error from fopen");
        errno;
        return 1;
    }
	
    while (!feof(f)) {
        c = (unsigned char) fgetc(f);
        if (c > 0 && c < 128)
            printf("%c", (int) c);
        else if (c >= 128) {        /* EOF = -1  -->  255 for unsigned char */
            fprintf(stderr, "Non-ascii character detected\n");
            return(1);
        }
    }

    fclose(f);
    return 0;
}

