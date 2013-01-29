#include <stdio.h>

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

./ascii-only ascii-only.c

-------------------------------------

qemu-system-mips -m 256M -M malta -kernel vmlinux-2.6.26-2-4kc-malta -hda debian_lenny_mips_standard.qcow2 -append "root=/dev/hda1 console=ttyS0" -nographic   
                                                                                
-m         : ram                                                                
-M         : machine                                                            
-hda       : disk image                                                         
-append    : kernel command line                                                
-nographic : no graphical output                                                
                                                                                
malta refers to The MIPS Malta prototype board 

-------------------------------------

1) Work on MIPS!

2) Better error messages...
     file not found
     can't open file
     usage

3) Specify infile and outfile

4) Specify log file
     infile
     outfile
     date
     line:character_position non-ascii (in hex or decimal)

5) Option for skiping or replacing non-ascii
   (can specify the replacement as char, decimal, or hex)

*/
