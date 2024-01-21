#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>
#include <stdbool.h>

#include "functions.h"

node ** csv_function(int argc, char *argv[])
{
	flags flag = initialize_flags();
	split = initialize_split();
	while (1)
	{
		int c = getopt_long(argc, argv, "w::s:c:r:l::", long_options, &option_index);
		
		if (argc == 1)
		{
			check_functionality = false;
		}

		if(c == -1)
		{
			break;
		}

		switch (c)
		{
			case 'w':
			{
				flag.w_flag = 1;		
				
				check_general_optarg(optarg, 'w');

				input_file_name = optarg ? &optarg[1] : default_file_name;	
				break;
			}

			case 's':
			{
				flag.s_flag = 1;

				check_general_optarg(optarg, 's');

				input_file_name = &optarg[1];	
				break;
			}

			case 'c':
			{
				flag.c_flag = 1;

				check_columns(optarg, master_current, 'c');
				break;
			}

			case 'r':
			{	
				flag.r_flag = 1;

				check_rows(optarg, master_current, 'r');
				break;
			}
			case 0:
			{
				/* Usage long name will get handled here */
				break;
			}
		}
	}
	if (check_functionality == true && argc >= 1)
	{
		excess_arguments(optind, argc);
		check_read_OR_write(flag);
	/*-----------------------------------------------------------*/	
		set_up_write(flag, input_file_name, default_file_name, master_current);		/*Check correct usage '-w'*/
																					/*&&*/
		implement_write(columns, rows, &inptr, master_current);						/*Implementing it*/
	/*-----------------------------------------------------------*/

	/*-----------------------------------------------------------*/	
		set_up_scan(flag, input_file_name, master_current);							/*Check correct usage '-s'*/
																					/*&&*/
		return implement_scan(split, flag, &inptr, master_current);								/*Implementing it*/
	/*-----------------------------------------------------------*/
	}
	else
	{
		print_error(69);
		exit(1);
	}
}
