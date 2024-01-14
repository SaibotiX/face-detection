typedef struct
{
	int l_flag;
	int w_flag;
	int s_flag;
	int c_flag;
	int r_flag;
} flags;

flags flag;

static struct option long_options[] =
{
	{"log-file", optional_argument, &flag.l_flag, 1},
	{"write", optional_argument, &flag.w_flag, 1},
	{"scan", required_argument, &flag.s_flag, 1},
	{"column", required_argument, &flag.c_flag, 1},
	{"rows", required_argument, &flag.r_flag, 1},
	{0,0,0,0}
};

int i_row_l, i_row_r;
int i_column_l = 0, i_column_r = 0, rows = 0;
int len = 0, option_index = 0, parse = 0, once = 0;

char *input_file_name = NULL;
char *default_file_name = "document.csv";
char *log_file_name = NULL;

FILE *inptr = NULL;

flags initialize_flags(void)
{
	flag.l_flag = 0;
	flag.w_flag = 0;
	flag.s_flag = 0;
	flag.c_flag = 0;
	flag.r_flag = 0;
	return flag;
}

void print_error(int error_number)
{
	printf("Help = '-h'\n");

	switch(error_number)
	{
		case 1:
		{	
			printf("After an [Argument] a '=' is required\n");
			break;
		}
		case 2:
		{
			printf("Define your value after the '='\n");
			break;
		}
		case 3:
		{
			printf("Only integers allowed\n");
			break;
		}
		case 4:
		{
			printf("Only one ':' allowed\n");
			break;
		}
		case 5:
		{
			printf("Specify only 'w' or 's', not both\n");
			break;
		}
		case 6:
		{
			printf("You got to specify the name\n");
			break;
		}
		case 7:
		{
			printf("Minimal Arguments for writing a .csv file are '-w' '-c' and '-r'\n");
			break;
		}
		case 8:
		{
			printf("Could not open file\n");
			break;
		}
	}
}

void check_general_optarg(char *optarg)
{
	if(optarg != NULL)
		{
			if(optarg[0] != '=')
			{
				print_error(1);
				exit(1);
			}
			if(strlen(optarg) == 1)
			{
				print_error(2);
				exit(1);
			}
		}
	else
	{
		print_error(6);
		exit(1);
	}
}

void check_int_semicolon_optarg(char *optarg)
{
	for (int i = 1, n = strlen(optarg); i < n; i = i + 1)
		{
			if (!isdigit(optarg[i]) && optarg[i] != ':')
			{
				print_error(3);
				exit(1);
			}
			else if(optarg[i] == ':')
			{
				if(once == 1)
				{
					print_error(4);
					exit(1);
				}

				once = 1;
				parse = i;
				optarg[i] == '\0';
			}
		}
}

void check_columns(char *optarg)
{
	check_general_optarg(optarg);

	check_int_semicolon_optarg(optarg);
}

void check_rows(char *optarg)
{
	check_general_optarg(optarg);

	check_int_semicolon_optarg(optarg);
}

void excess_arguments(int optind, int argc)
{
	while(optind < argc)
	{
		optind = optind + 1;
	}
}

void check_read_OR_write(flags flag)
{
	if(flag.w_flag == 1 && flag.s_flag == 1)
	{
		print_error(5);
		exit(1);
	}
}

void check_file(FILE *inptr)
{
	if (inptr == NULL)
	{
		print_error(8);
		exit(1);
	}
}

void implement_write(flags flag, char *input_file_name, char *default_file_name)
{
	if(flag.w_flag == 1 && flag.c_flag == 1 && flag.c_flag == 1)
	{
		if (input_file_name != NULL)
		{
			inptr = fopen(input_file_name, "w");

			check_file(inptr);

			fclose(inptr);
		}
		else
		{
			 inptr = fopen(default_file_name, "w");

			 check_file(inptr);

			 fclose(inptr);
		}
	}
	else
	{
		print_error(7);
		exit(1);
	}
}

void check_scan(void)
{
}
