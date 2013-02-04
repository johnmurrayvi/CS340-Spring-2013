#include <stdio.h>

int main(int argc, char **argv)
{
    int i; FILE *f;
	if (argc < 2){
		fprintf(stderr, "usage: ./ascii-only filename\n");
		exit(1);
	}
	
	f = fopen(*++argv,"r");

    if (!f) {
        perror();
        return 1;
    }
	
    while (EOF != fscanf(f,"%c",(char *)&i))
        if (!(i&128)) printf("%c",(char)(127&i));
    fclose(f);
    return 0;
}
