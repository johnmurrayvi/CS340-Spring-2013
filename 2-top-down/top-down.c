
#include "top-down.h"

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

int determine_replacement(int i, int argc, char **argv)
{
    int n, R, res;

    /* Make sure there is a value following the -r flag */
    if (argv[i]) {
        switch(argv[i][1]) {
            /* Check if hex was given */
            case 'X':
            case 'x':
                res = sscanf(argv[i],"%x%n", &R,&n);
                /* 
                 * Error check the hex read: 
                 * range of R, bad specification, or
                 * strlen > 4 ==> R > 1 byte,
                 * trailing bytes, etc. 
                 * break if it fails any
                 */
                if ((res !=  1) || (argv[i][0] != '0') || 
                    (R < 0) || (R > 255) ||                 
                    (strlen(argv[i]) > 4) || (argv[i][n]))  
                    break;                  
                /* It is hex, so return it. */
                 return R;

            /* Check if it is character (via trailing '\0') */
            case  0 :
                R = argv[i][0];
                return R;

            /* Check for decimal replacement */
            default :
                /* Error check the decimal read break if it fails any */
                res = sscanf(argv[i],"%d.%n", &R,&n);
                if ((res != 1) || (strlen(argv[i]) > 4) ||
                    (R < 0) || (R > 255) || (argv[i][n]))
                    break;
                /* Return the decimal replacement*/
                return R;
        }
    }
    /* If we got here, there was a problem so return an error */
    return -1;
}

int option(char *s, int argc, char **argv, int *flags)
{
    int i, j;

    /* Traverse arguments and  */
    for (i = 0, j = 1; j < argc; j++) {
        if (!strcmp(s, argv[j])) {
            /* error if multiple occurrences of s */
            if (i)                      
                *flags = argc;
            else {
                i = j+1;            /* option value is argv[*i] */
                *flags += 2;        /* account for flag and its val */
            }
        }
    }
    /* return 0 or the index of option value */
    return i;
}

int args_ok(int argc, char **argv, int *R, iofiles *f)
{
    int i, flags;

    /* flags is the count of args encountered, or argc if error */
    flags = 0;

    /* Check if infile file was specified */
    i = option("-i", argc, argv, &flags);
    if (i != 0)
        f->Infile = (void *)argv[i];

    /* Check if output file was specified */
    i = option("-o", argc, argv, &flags);
    if (i != 0)
        f->Outfile = (void *)argv[i];

    /* Check if log flag was specified */
    i = option("-l", argc, argv, &flags);
    if (i != 0)
        f->Logfile = (void *)argv[i];

    /* Check if replacement was specified */
    i = option("-r", argc, argv, &flags);
    if (i != 0) {
        /* 
         * Figure out how the replacement was specified
         * i is set to -1 for an error condition
         */
        *R = determine_replacement(i, argc, argv);
        if(i == -1)
            flags = argc;
    }

    /* See if all args are accounted for and no errors have occured */
    if (1+flags == argc)
        return 1;

    /* If we get here, something isn't right with the arguemtns */
    show_usage();
    return 0;
}

int io_files_ok(iofiles *f)
{
    time_t t;

    /* Check for input file */
    if (!(f->Infile))
        f->In = stdin;
    else {
        f->In = fopen(f->Infile, "r");
        if (f->In == NULL) {
            perror("infile");
            return 0;
        }
    }
    /* Check for output file */
    if (!(f->Outfile))
        f->Out = stdout;
    else {
        f->Out = fopen(f->Outfile, "w");
        if (f->Out == NULL) {
            perror("outfile");
            return 0;
        }
    }
    /* Check for logfile */
    if (f->Logfile) {
        f->Log = fopen(f->Logfile, "a");
        if (f->Log == NULL) {
            perror("logfile");
            return 0;
        }
        /* Get the current time and print info to log */
        t = time(NULL);
        fprintf(f->Log,"%s\n", f->Infile);
        fprintf(f->Log,"%s\n", f->Outfile);
        fprintf(f->Log,"%s\n", ctime(&t));
        fprintf(f->Log,"Ln [Pos]:  <Val>\n");
    }
    return 1;
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

    R = -1;     /* Replacement, -1 means none */
    L = 0;      /* Line number */
    P = 0;      /* Character position in line */
    initialize_iofiles(&f);

    /* Error checking */
    aok = args_ok(argc, argv, &R, &f);
    iok = io_files_ok(&f);
    if (aok && iok) {
        C = getc(f.In);
        while (C != EOF) {
            /* Make sure C fits in 1 byte */
            if (!(128 & C)) {
                fputc(C, f.Out);
                P++;                /* keep track of position */
                if (C == '\n') {
                    L++;          /* keep track of line */
                    P = 0;        /* reset position if new line */
                }
            } else {
                /* Need to replace the character just read */
                if (R != -1)
                    fputc(R, f.Out);
                /* Log the bad character if needed */
                if (f.Log)
                    fprintf(f.Log,"%lu [%lu]:  0x%x\n",L,P,C);
                P++;
            }
            C = getc(f.In);
        }
        /* Clean up files and return success */
        if (f.In  != stdin ) fclose(f.In);
        if (f.Out != stdout) fclose(f.Out);
        if (f.Log != NULL) fclose(f.Log);

        return 0;
    }
    /* Fail */
    return 1;
}
