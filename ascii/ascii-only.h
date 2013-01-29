#include <stdio.h>
#include <stdlib.h>

int file_exists(char *x)
{
  char s[128];

  sprintf(s,"ls %s &>/dev/null",x);
  if (system(s)){
    printf("file %s not found\n", x);    
    return 0;
  }
  return 1;
}

FILE *open_for_reading(char *x)
{
  FILE *f = fopen(x,"r");

  if (!f){
    printf("can't open %s for reading\n", x);    
  }
  return f;
}

#define fopen(x, ...)                                   \
          ((file_exists(x))? open_for_reading(x): NULL)

