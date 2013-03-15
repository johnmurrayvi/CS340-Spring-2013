/*#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
*/
#include "top-down.h"
/*typedef struct F {
    char *Infile;
    char *Outfile;
    char *Logfile;
    FILE *In;
    FILE *Out;
    FILE *Log;
} iofiles;
*/
void show_usage()
{
    printf("\nUsage: "
                 "ascii-only [-r replacement] [-l log_file ] [-i input_file] [-o output_file]\n\n"
                 "  Missing input/output files default to stdin/stdout.\n"
                 "  Replacement is one of the following: \n"
                 "   character\n"
                 "   hex constant ( i.e., 0x3F )\n"
                 "   integer from 0 to 255 with decimal point ( i.e., 132. )\n\n");
}

/*this function is needed, unless we rewrite the switch statement. Vose wrote it to
return as soon as a replacement character is found, and 'break' the switch statement
if a read failed. If the program makes it through the switch statement an error occured.
If this were written in args_ok the returns would have to be taken out, but then the
program would always make it through the switch statement and the error would always
"occur". Having it also follows the top-down coding style that Vose wants us to use.
--just leaving this comment in here temporarily, to explain why Jordan added
this function back*/
int determine_replacement(int i, int argc, char **argv)
{
  int n, R, res;

  if (argv[i]){                                    // -r option is followed by a value
	switch(argv[i][1]){                            // second character of value
	case 'X':
	case 'x':                                      // hex replacement
	  res = sscanf(argv[i],"%x%n", &R,&n);
	  if ((res !=  1)||                            // argv[i] won't read as hex
		  (R < 0)||                                // R too small
		  (R > 255)||                              // R too big
          (strlen(argv[i]) > 4)||                  // argv[i] too long
          (argv[i][0] != '0')||                    // argv[i] doesn't begin "0x" or "0X"
          (argv[i][n]))                            // argv[i] has trailing junk
        break;
      return R;                                    // return the replacement if it was read
	  
    case  0 :                                      // character replacement
      R = argv[i][0];
      return R;                                    // return the replacement if it was read
	  
    default :                                      // decimal replacement
      res = sscanf(argv[i],"%d.%n", &R,&n);
	  if ((res != 1)||  // argv[i] won't read as decimal
          (R < 0)||                                // R too small
          (R > 255)||                              // R too big
          (strlen(argv[i]) > 4)||                  // argv[i] too long
          (argv[i][n]))                            // argv[i] has trailing junk
        break;
      return R;                                    // return the replacement if it was read
    }
  }
  
  return -1;                                    // error condition
}

int option(char *s, int argc, char **argv, int *flags)
{
    int i, j;

    for (i = 0, j = 1; j < argc; j++)      // traverse command line arguments
        if (!strcmp(s, argv[j])){             // j_th argument matches option s
            if (i)                             // multiple occurrences of s
                *flags = argc;                         // error condition
            else {
                i = j+1;                         // option value is argv[*i]
                *flags += 2;                           // account for two arguments
            }
        }
		
    return i;                              // zero, or else index of option value
}

int args_ok(int argc, char **argv, int *R, iofiles *f)
{
    int i, flags;

    flags = 0;			                           // flags is the count of args encountered, or argc if error

	i = option("-i", argc, argv, &C);				            // infile (if problem, set C to argc)
    if (i != 0)
        f->Infile = (void *)argv[i];           // filename

	i = option("-o", argc, argv, &C);				            // outfile (if problem, set C to argc)
    if (i != 0)
        f->Outfile = (void *)argv[i];          // filename

	i = option("-l", argc, argv, &C);		                    // logfile (if problem, set C to argc)
    if (i != 0)
        f->Logfile = (void *)argv[i];          // filename

	i = option("-r", argc, argv, &C);                           // replacement (if problem, set C to argc)
    if (i != 0)
	{
		*R = determine_replacement(i, argc, argv);	            // determine replacement character (-1 indicates error)
        if(i == -1)
            C = argc;
	}

    if (1+flags == argc)                                            // no errors and all arguments acounted for
        return 1;                              // success

    show_usage();
    return 0;                                  // failure
}

int io_files_ok(iofiles *f)
{
    time_t t;
    
    t = time(NULL);                            // get current time

    if (!(f->Infile))                          // no infile specified
        f->In = stdin;                             // stdin is default
    else {
        f->In = fopen(f->Infile, "r");         // use infile specified
        if (f->In == NULL){                        // cant open infile
            perror("infile");                          // show error
            return 0;                                  // return failure
        }
    }
    if (!(f->Outfile))                         // no outfile specified
        f->Out = stdout;                           // stdout is default
    else {
        f->Out = fopen(f->Outfile, "w");           // use outfile specified
        if (f->Out == NULL){                       // cant open outfile
            perror("outfile");                         // show error
            return 0;                                  // return failure
        }
    }
    if (f->Logfile){                           // logfile specified
        f->Log = fopen(f->Logfile, "w");
        if (f->Log == NULL){                       // cant open logfile
            perror("logfile");                         // show error
            return 0;                                  // return failure
        }
		
        fprintf(f->Log,"%s\n", f->Infile);         // put infile name in logfile
        fprintf(f->Log,"%s\n", f->Outfile);        // put outfile name in logfile
        fprintf(f->Log,"%s\n", ctime(&t));         // put date in logfile
        fprintf(f->Log,"Ln [Pos]:  <Val>\n");
    }
    return 1;                                  // return success
}

void initialize_iofiles(iofiles *f)
{
    f->Infile  = 0;
    f->Outfile = 0;
    f->Logfile = 0;
    f->In = NULL;
    f->Out = NULL;
    f->Log = NULL;
}

int main(int argc, char **argv)
{
    int C, R, aok, iok;
    unsigned long L, P;
    iofiles f;

    R = -1;                     // replacement byte (-1 signifies no replacement)
    L = 0;
    P = 0;
    initialize_iofiles(&f);

    aok = args_ok(argc, argv, &R, &f);                 // check syntax
	iok = io_files_ok(&f);                             // open files
	if (aok && iok)                                    // if everything is ok
	{
		C = getc(f.In));
        while ((C != EOF) {             // C is the character (byte) read
            if (!(128 & C)) {           // C is an ascii character
                fputc(C, f.Out);  // output C
                P++;            // keep track of character position for logging
                if (C == '\n'){ // new line
                    L++;          // keep track of line number for logging
                    P = 0;        // reset character position for logging
                }
            } else {
                if (R != -1)                            // -1 signifies no replacement
                    fputc(R, f.Out);                        // output R
                if (f.Log)                                // if log
                    fprintf(f.Log,"%lu [%lu]:  0x%x\n",L,P,C); // line_number: character_position C (in hex)
                P++;                                    // keep track of character position for logging
            }
			C = getc(f.In);
        }
        if (f.In  != stdin ) fclose(f.In);  // close infile
        if (f.Out != stdout) fclose(f.Out); // close outfile
        if (f.Log != NULL) fclose(f.Log); // no logging is indicated by Log = NULL

        return 0;                                 // success
    }
    return 1;                                   // error
}
