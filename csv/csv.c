#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>
#include <stdbool.h>

#include "functions.h"

int main(int argc, char *argv[])
{
	while (1)
	{

		flags flag = initialize_flags();

		int c = getopt_long(argc, argv, "w::s:c:r:l::", long_options, &option_index);

		if(c == -1)
		{
			break;
		}

		switch (c)
		{
			case 'w':
			{
				flag.w_flag = 1;		
				
				check_general_optarg(optarg);

				input_file_name = optarg ? &optarg[1] : default_file_name;	
				break;
			}

			case 's':
			{
				flag.s_flag = 1;

				check_general_optarg(optarg);

				input_file_name = optarg;	
				break;
			}

			case 'c':
			{
				flag.c_flag = 1;

				check_columns(optarg);

				i_column_l = atoi(&optarg[1]);
				i_column_r = atoi(&optarg[parse + 1]);
				break;
			}

			case 'r':
			{	
				flag.r_flag = 1;

				check_rows(optarg);

				i_row_l = atoi(&optarg[1]);
				i_row_r = atoi(&optarg[parse + 1]);

				break;
			}

			case 'l':
			{	
				flag.l_flag = 1;
				
				check_general_optarg(optarg);

				log_file_name = optarg ? &optarg[1] : "csv.log";
				break;
			}
			
			case 0:
			{
				printf("You did use the long name\n");
				break;
			}

			case '?':
			{
				printf("Something went wrong\n");
				return 1;	
			}
		}
	}

	excess_arguments(optind, argc);
	
	check_read_OR_write(flag);
	
	implement_write(flag, input_file_name, default_file_name);

	return 0;
}
