#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>
#include <stdbool.h>

#include "functions.h"

result csv_function(int argc, char *argv[])
{
	/* Global Variables */
	buffer = NULL;
	flag = initialize_flags();

	/* Local Variables */
	bool check_functionality = true;

	char *input_file_name = NULL;
	char *default_file_name = "document.csv";
	
	char master_current = 0;

	int option_index = 0;

	splits split = initialize_split();
	layout table;

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
				initialize_master_current(&master_current, 'w');

				flag.w_flag = 1;
				
				check_general_optarg(optarg, 'w');

				input_file_name = optarg ? &optarg[1] : default_file_name;	
				break;
			}

			case 's':
			{
				initialize_master_current(&master_current, 's');

				flag.s_flag = 1;

				check_general_optarg(optarg, 's');

				input_file_name = &optarg[1];	
				break;
			}

			case 'c':
			{
				flag.c_flag = 1;

				check_master_current(master_current);

				check_general_optarg(optarg, 'c');

				if(master_current == 's') {
					int *position_semicolons = track_semicolons();
					define_split(optarg, &split, 'c', position_semicolons);
				}
				else if(master_current == 'w') {
					define_table(&table, 'c');
				}
				break;
			}

			case 'r':
			{	
				flag.r_flag = 1;

				check_master_current(master_current);

				check_general_optarg(optarg, 'r');

				if(master_current == 's') {
					int *position_semicolons = track_semicolons();
					define_split(optarg, &split, 'r', position_semicolons);
				}
				else if(master_current == 'w') {
					define_table(&table, 'r');
				}
				break;
			}
			case 0:
			{
				/* Usage long name will get handled here */
				break;
			}
			case '?':
			{
				exit(1);
			}
		}
	}
	if (check_functionality == true && argc >= 1)
	{
		excess_arguments(optind, argc);
		check_split(split);
		check_read_OR_write(flag);
	/*-----------------------------------------------------------*/	
		set_up_write(flag, input_file_name, default_file_name, master_current);		/*Check correct usage '-w'*/
																					/*&&*/
		implement_write(table, master_current);						/*Implementing it*/
	/*-----------------------------------------------------------*/

	/*-----------------------------------------------------------*/	
		set_up_scan(flag, input_file_name, master_current);							/*Check correct usage '-s'*/
																					/*&&*/
		return implement_scan(split, master_current);								/*Implementing it*/
	/*-----------------------------------------------------------*/
	}
	else
	{
		print_error(69);
		exit(1);
	}
}
