#include "ascii-only.h"

int main(int argc, char **argv)
{
    int i;
	char file_check[2];
	
	if (argc < 2 || argc > 3){
		fprintf(stderr, "usage info\n----------\ninfile->outfile: ./ascii-only infile outfile\nstdin->outfile: ./ascii-only --outfile\nstdin->stdout: ./ascii-only --\ninfile->stdout: ./ascii-only outfile\n");
		exit(0);
	}

	arg_check(argv);

	/*	below is acutal code for arg_check(char **argv) above. I just have it commented out because I didn't know if we wanted all the code explicitly in main or if we wanted it in a function in ascii-only.h. I just left the code so if we wanted it in main we could just un-comment it.*/
	
	
	/*file_check[0] = (char)argv[1][0];
	file_check[1] = (char)argv[1][1];

	if(file_check[0] == '-' && file_check[1] == '-' && argv[1][2] == 0)	{
	
    while (EOF != fscanf(stdin,"%c",(char *)&i))	{
        if (!(i&128))	{
			printf("%c",(char)(127&i));
			}
		}
	}	
	else if(file_check[0] == '-' && file_check[1] == '-' && argv[1][2] != 0 && argv[2] == NULL)	{

		FILE *outfile;
		char *outname;
		int i;
		int outlength = 0;
		
		for(i = 2; ; i++)	{
			if(argv[1][i] == 0)	{
				break;
			}
			outlength++;
		}
	
		outlength = 0;
		outname = (char *)malloc(sizeof(char) * (outlength + 1));
		for(i = 2; ; i++)	{
			outname[outlength++] = argv[1][i];
			if(argv[1][i] == 0)	{
				break;
			}
		}
		outfile = fopen(outname, "w");
		if(!outfile)	{
			perror(outname);
			exit(0);
		}
		while (EOF != fscanf(stdin,"%c",(char *)&i))	{
		    if (!(i&128))	{
				fprintf(outfile, "%c",(char)(127&i));
				}
			}
		fclose(outfile);
	}
	else if(argv[1] != NULL && argv[2] != NULL)	{

		FILE *infile, *outfile;

		infile = fopen(argv[1], "r");
		if(!infile)	{
			perror(argv[1]);
			exit(0);
		}
		outfile = fopen(argv[2], "w");
		if(!outfile)	{
			perror(argv[2]);
			exit(0);
		}
		
		while (EOF != fscanf(infile,"%c",(char *)&i))	{
		    if (!(i&128))	{
				fprintf(outfile, "%c",(char)(127&i));
				}
			}
		fclose(infile);
		fclose(outfile);
		}
	else	{
		fprintf(stderr, "usage info\n----------\ninfile->outfile: ./ascii-only infile outfile\nstdin->outfile: ./ascii-only --outfile\nstdin->stdout: ./ascii-only --\n");
	}*/

		return 0;
}
