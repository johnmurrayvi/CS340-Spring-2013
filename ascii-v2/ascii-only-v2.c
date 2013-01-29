#include "ascii-only.h"

int main(int argc, char **argv)
{
  int i;  FILE *f = fopen(*++argv,"r");

  if (!f){
    printf("usage ./ascii-only filename\n");
    return 1;
  }
  while (EOF != fscanf(f,"%c",(char *)&i))
    if (!(i&128)) printf("%c",(char)(127&i));
  fclose(f);
  return 0;
}
/*
gcc -Wall -gdwarf-3 -o ascii-only ascii-only.c
*/
