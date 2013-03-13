#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

typedef struct F {
    char *Infile;
    char *Outfile;
    char *Logfile;
    FILE *In;
    FILE *Out;
    FILE *Log;
} iofiles;

void show_usage();
int option(char *, int, char **, int *); 
int args_ok(int , char **, int *, iofiles *);
int io_files_ok(iofiles *);
void initialize_iofiles(iofiles *f);

