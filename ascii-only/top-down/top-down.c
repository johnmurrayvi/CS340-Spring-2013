#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

/*
Globals -- gasp -- we're done for!
*/

unsigned long L,P;                        // line counter, character position
int           C;                          // option counter or error condition or character read
int           R = -1;                     // replacement byte (-1 signifies no replacement)
char         *Infile, *Outfile, *Logfile; // names for: infile, outfile, logfile
FILE         *In, *Out, *Log;             // file pointers for: infile, outfile, logfile

void determine_replacement(int i, int argc, char **argv)
{
  int n;

  if (argv[i]){                                    // -r option is followed by a value
    switch(argv[i][1]){                            // second character of value
    case 'X':
    case 'x':                                      // hex replacement
      if ((1 != sscanf(argv[i],"%x%n", &R,&n))||   // argv[i] won't read as hex
          (R < 0)||                                // R too small
          (R > 255)||                              // R too big
          (strlen(argv[i]) > 4)||                  // argv[i] too long
          (argv[i][0] != '0')||                    // argv[i] doesn't begin "0x" or "0X"
          (argv[i][n]))                            // argv[i] has trailing junk
        break;
      return;
    case  0 :                                      // character replacement
      R = argv[i][0];
      return;
    default :                                      // decimal replacement
      if ((1 != sscanf(argv[i],"%d.%n", &R,&n))||  // argv[i] won't read as decimal
          (R < 0)||                                // R too small
          (R > 255)||                              // R too big
          (strlen(argv[i]) > 4)||                  // argv[i] too long
          (argv[i][n]))                            // argv[i] has trailing junk
        break;
      return;
    }
  }
  C  = argc;                                       // error condition
}

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

int option(char *s, int *i, int argc, char **argv)
{
  int j;

  for (*i = 0, j = 1; j < argc; j++)      // traverse command line arguments
    if (!strcmp(s, argv[j])){             // j_th argument matches option s
      if (*i)                             // multiple occurrences of s
        C = argc;                         // error condition
      else {
        *i = j+1;                         // option value is argv[*i]
        C += 2;                           // account for two arguments
      }
    }
  return *i;                              // zero, or else index of option value
}

int command_line_arguments_ok(int argc, char **argv)
{
  int i;  C = 0;                          // C counts arguments or signals error condition

  if (option("-i", &i, argc, argv))       // infile (if problem, set C to error condition)
    Infile = (void *)argv[i];             // filename

  if (option("-o", &i, argc, argv))       // outfile (if problem, set C to error condition)
    Outfile = (void *)argv[i];            // filename

  if (option("-l", &i, argc, argv))       // logfile (if problem, set C to error condition)
    Logfile = (void *)argv[i];            // filename

  if (option("-r", &i, argc, argv))       // replacement (if problem, set C to error condition)
    determine_replacement(i, argc, argv); // R = replacement (if problem, set C to error condition)

  if (1+C == argc)                        // no errors and all arguments acounted for
    return 1;                             // success

  show_usage();
  return 0;                               // failure
}

#if 0

/*
Why are the comments in io_files_ok really good?
Because they can be used as a top-down specification for the computation!
*/

int io_files_ok()
{
  time_t t = time(NULL);                        // get current time

  if (!Infile)                                  // no infile specified
    In = stdin;                                 // stdin is default
  else if (!(In = fopen(Infile, "r"))){         // cant open infile
    perror("infile");                           // show error
    return 0;                                   // return failure
  }    
  if (!Outfile)                                 // no outfile specified
    Out = stdout;                               // stdout is default
  else if (!(Out = fopen(Outfile, "w"))){       // cant open outfile
    perror("outfile");                          // show error
    return 0;                                   // return failure
  }
  if (Logfile){                                 // logfile specified
    if (!(Log = fopen(Logfile, "w"))){          // cant open logfile
      perror("logfile");                        // show error
      return 0;                                 // return failure
    }
    fprintf(Log,"%s\n", Infile);                // put infile name in logfile
    fprintf(Log,"%s\n", Outfile);               // put outfile name in logfile
    fprintf(Log,"%s\n", ctime(&t));             // put date in logfile
  }
  return 1;                                     // return success
}

#else

/*
Here is the proof...
*/
#define get_current_time            time_t t = time(NULL)
#define no_infile_specified         !Infile
#define stdin_is_default            In = stdin
#define cant_open_infile            !(In = fopen(Infile, "r"))
#define show_error(x)               perror(x)
#define return_success              return 1
#define return_failure              return 0
#define no_outfile_specified        !Outfile
#define stdout_is_default           Out = stdout
#define cant_open_outfile           !(Out = fopen(Outfile, "w"))
#define logfile_specified           Logfile
#define cant_open_logfile           !(Log = fopen(Logfile, "w"))
#define put_infile_name_in_logfile  fprintf(Log,"%s\n", Infile)
#define put_outfile_name_in_logfile fprintf(Log,"%s\n", Outfile)
#define put_date_in_logfile         fprintf(Log,"%s\n", ctime(&t)) 

int io_files_ok()
{
  get_current_time;

  if (no_infile_specified)
    stdin_is_default;
  else if (cant_open_infile){
    show_error("infile");
    return_failure;
  }    
  if (no_outfile_specified)
    stdout_is_default;
  else if (cant_open_outfile){
    show_error("outfile");
    return_failure;
  }
  if (logfile_specified){
    if (cant_open_logfile){
      show_error("logfile");
      return_failure;
    }
    put_infile_name_in_logfile;
    put_outfile_name_in_logfile;
    put_date_in_logfile;
  }
  return_success;
}    

/*
This coding style separates the computation from its implementation.
You have to implement at some point -- the macros do that -- but the
separation is generally regarded as a good thing (using globals or
macros for the implementation is not).
*/
#endif

char can_read_character() // return 0 if EOF
{
  return ((C = getc(In)) != EOF);  // C = next character or EOF
}

int ascii_character() // return whether C is ascii
{
  return !(128&C);
}

void output_character() // output C, keep track of line number and character position
{
  fputc(C, Out);  // output C
  P++;            // keep track of character position for logging
  if (C == '\n'){ // new line
    L++;          // keep track of line number for logging
    P = 0;        // reset character position for logging
  }
}

void output_replacement_character() // output either R or else nothing, handle logging
{
  if (R != -1)                            // -1 signifies no replacement
    fputc(R, Out);                        // output R
  if (Log)                                // if log
    fprintf(Log,"%lu: %lu 0x%x\n",L,P,C); // line_number: character_position C (in hex)
  P++;                                    // keep track of character position for logging
}

void clean_up() // close opened files
{
  if (In  != stdin ) fclose(In);  // close infile
  if (Out != stdout) fclose(Out); // close outfile
  if (Log          ) fclose(Log); // no logging is indicated by Log = NULL
}

int main(int argc, char **argv)
{
  if (command_line_arguments_ok(argc,argv) && // check syntax
      io_files_ok()){                         // open files
    
    while (can_read_character())              // C is the character (byte) read
      if (ascii_character())                  // C = b7 b6 b5 b4 b3 b2 b1 b0 has zero bit b7
        output_character();                   // (as specified by command line arguments)
      else
        output_replacement_character();       // the replacement can be nothing (no output)

    clean_up();                               // close opened files
    return 0;                                 // success
  }
  return 1;                                   // error
}

/*
gcc -Wall -g -o ascii-only top-down.c

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

5) Option for skipping or replacing non-ascii
   (can specify the replacement as char, decimal, or hex)

*/
