#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    unsigned char c;
    int i;
    FILE *f;
	
    if (argc < 2) {
		fprintf(stderr, "usage: ./ascii-only filename\n");
		exit(1);
	}
	
	f = fopen(*++argv,"r");

    if (!f) {
        perror("error from fopen:");
        return 1;
    }
	
    while (!feof(f)) {
        c = (unsigned char) fgetc(f);
        if (c > 0 && c < 128)
            printf("%c", (int) c);
    }

    fclose(f);
    return 0;
}
