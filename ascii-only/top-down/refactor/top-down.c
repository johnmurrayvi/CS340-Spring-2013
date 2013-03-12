#include <stdio.h>
#include <errno.h>
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

int option(char *s, int argc, char **argv, int *C)
{
    int i, j;

    for (i = 0, j = 1; j < argc; j++)      // traverse command line arguments
        if (!strcmp(s, argv[j])){             // j_th argument matches option s
            if (i)                             // multiple occurrences of s
                *C = argc;                         // error condition
            else {
                i = j+1;                         // option value is argv[*i]
                *C += 2;                           // account for two arguments
            }
        }
    return i;                              // zero, or else index of option value
}

int args_ok(int argc, char **argv, int *R, iofiles *f)
/*TODO: add error checks to the sscanf calls, and possibly rework the if statements in the -r option
	Probably worth adding back the determine_replacement function. Keeps with the top-down
	methodology and lets that switch statement execute properly*/
{
    int i, n, C;

	C = 0;					//C is the count of args encountered, or argc if error
	
    if ((i = option("-i", argc, argv, &C)) != 0)       // infile (if problem, set C to argc)
        f->Infile = (void *)argv[i];           // filename

    if ((i = option("-o", argc, argv, &C)) != 0)       // outfile (if problem, set C to argc)
        f->Outfile = (void *)argv[i];          // filename

    if ((i = option("-l", argc, argv, &C)) != 0)       // logfile (if problem, set C to argc)
        f->Logfile = (void *)argv[i];          // filename

    if ((i = option("-r", argc, argv, &C)) != 0) {      // replacement (if problem, set C to argc)
        if (argv[i]){                       // -r option is followed by a value
            switch(argv[i][1]){                 // second character of value
            case 'X':
            case 'x':                           // hex replacement
				/*TODO: If we keep this not in a seperate function we need rewrite this if
				statement. Right now when it fails R might still be set and then the break
				statement won't execute. Strange*/
                if ((1 != sscanf(argv[i],"%x%n", R, &n))||   // argv[i] won't read as hex
                        (*R < 0)||                                // R too small
                        (*R > 255)||                              // R too big
                        (strlen(argv[i]) > 4)||                  // argv[i] too long
                        (argv[i][0] != '0')||                    // argv[i] doesn't begin "0x" or "0X"
                        (argv[i][n]))                            // argv[i] has trailing junk
            		break;
            case null:                                      // character replacement
                *R = argv[i][0];
				break;
            default :                                      // decimal replacement
				/*TODO: See the TODO comment about the if statement above*/
                if ((1 != sscanf(argv[i],"%d.%n", R, &n))||  // argv[i] won't read as decimal
                        (*R < 0)||                                // R too small
                        (*R > 255)||                              // R too big
                        (strlen(argv[i]) > 4)||                  // argv[i] too long
                        (argv[i][n]))                            // argv[i] has trailing junk
                    break;
            }
        }
		/*TODO: If we keep this not in a seperate function this will always execute when a -r option
		is used, and the function will always return an error*/
        C  = argc;                                       // error condition
    }

    if (1+C == argc)                        // no errors and all arguments acounted for
        return 1;                             // success

    show_usage();
    return 0;                               // failure
}

int io_files_ok(iofiles *f)
{
    time_t t = time(NULL);                        // get current time

    if (!(f->Infile))                                  // no infile specified
        f->In = stdin;                                 // stdin is default
    else if (!(f->In = fopen(f->Infile, "r"))){         // cant open infile
        perror("infile");                           // show error
        return 0;                                   // return failure
    }    
    if (!f->Outfile)                                 // no outfile specified
        f->Out = stdout;                               // stdout is default
    else if (!(f->Out = fopen(f->Outfile, "w"))){       // cant open outfile
        perror("outfile");                          // show error
        return 0;                                   // return failure
    }
    if (f->Logfile){                                 // logfile specified
        if (!(f->Log = fopen(f->Logfile, "w"))){          // cant open logfile
            perror("logfile");                        // show error
            return 0;                                 // return failure
        }
        fprintf(f->Log,"%s\n", f->Infile);                // put infile name in logfile
        fprintf(f->Log,"%s\n", f->Outfile);               // put outfile name in logfile
        fprintf(f->Log,"%s\n", ctime(&t));             // put date in logfile
    }
    return 1;                                     // return success
}

void initialize_iofiles(iofiles *f)
{
	f->Infile  = 0;
	f->Outfile = 0;
	f->Logfile = 0;
}

int main(int argc, char **argv)
{
    int C, R;
    unsigned long L, P;
    iofiles f;

    R = -1;                     // replacement byte (-1 signifies no replacement)
    L = 0;
    P = 0;
	initialize_iofiles(&f);

    if (args_ok(argc,argv, &R, &f) && // check syntax
            io_files_ok(&f)){                         // open files
        
        while ((C = getc(f.In)) != EOF) {             // C is the character (byte) read
            if (!(128 & C)) {
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
                    fprintf(f.Log,"%lu: %lu 0x%x\n",L,P,C); // line_number: character_position C (in hex)
                P++;                                    // keep track of character position for logging
            }
        }
        if (f.In  != stdin ) fclose(f.In);  // close infile
        if (f.Out != stdout) fclose(f.Out); // close outfile
        if (f.Log) fclose(f.Log); // no logging is indicated by Log = NULL

        return 0;                                 // success
    }
    return 1;                                   // error
}
