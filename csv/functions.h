#define _STR(x) #x
#define STR(x) _STR(x)

#define MAX 20

typedef struct {
	int w_flag;
	int s_flag;
	int c_flag;
	int r_flag;
	int k_flag;
} flags;

flags flag;

typedef struct {
	int column_l;
	int column_m;
	int column_r;

	int row_l;
	int row_m;
	int row_r;
} splits;

splits split;

typedef struct node
{
	int input;
	struct node *next;
} node;

static struct option long_options[] = {
	{"write", optional_argument, &flag.w_flag, 1},
	{"scan", required_argument, &flag.s_flag, 1},
	{"column", required_argument, &flag.c_flag, 1},
	{"rows", required_argument, &flag.r_flag, 1},
	{0,0,0,0}
};

int rows;
int columns;
int len = 0, option_index = 0, parse[2], once_or_twice = 0;
int tmp = 0;
int node_iteration = 0;
int array_count = 0, count = 0;

char *input_file_name = NULL;
char *default_file_name = "document.csv";
char *buffer = NULL;

char master_current = 0; /*Check if it is [-w] or [-s]*/
char c = 0; /*iterating in fread*/

bool check_functionality = true;

FILE *inptr = NULL;

flags initialize_flags(void) {
	flags flag;
	flag.w_flag = 0;
	flag.s_flag = 0;
	flag.c_flag = 0;
	flag.r_flag = 0;
	flag.k_flag = 0;
	return flag;
}

splits initialize_split(void) {
	splits split;
	split.column_l = 0;
	split.column_m = 0;
	split.column_r = 0;
	split.row_l = 0;
	split.row_m = 0;
	split.row_r = 0;
	return split;
}

void print_error(int error_number) 
{
	check_functionality = false;
	printf("Help = '-h'\n");

	switch(error_number) {
		case 1: {	
			printf("After an [Argument] a '=' is required\n");
			break;
		}
		case 2: {
			printf("Define your value after the '='\n");
			break;
		}
		case 3: {
			printf("Only integers allowed\n");
			break;
		}
		case 4: {
			printf("Maximal two ':' allowed\n");
			break;
		}
		case 5: {
			printf("Specify only 'w' or 's', not both\n");
			break;
		}
		case 6: {
			printf("You got to specify the name\n");
			break;
		}
		case 7: {
			printf("Minimal Arguments for writing a .csv file are '-w' '-c' and '-r'\n");
			break;
		}
		case 8: {
			printf("Could not open file\n");
			break;
		}
		case 9: {
			printf("No ':' allowed with the option '-w'\n");
			break;
		}
		case 10: {
			printf("Minimal Arguments for scanning a .csv file are '-s' '-c- and '-r'\n");
			break;
		}
		case 11: {
			printf("Something went wrong, sorry.\n");
			break;
		}
		case 12: {
			printf("Your first specification should be 'w' or 's'\n");
			break;
		}
		case 13: {
			printf("Please the range correctly: [-c] or [-r] =[from]:[to]\n");
			break;
		}
		case 14: {
			printf("Provide one : if you want the columns and rows [from]:[to] or two : for [from input 1]:[to input 2]:[output] (This is handy for machine learning where you have multiple inputs but only one output\n");
			break;
		}
		case 15: {
			printf("Make sure to read only integers\n");
			break;
		}
		case 16: {
			printf("Make sure you have no words longer than 49 Characters\n");
			break;
		}
		case 69: {
			/* Nothing happens, except the first notification */
			break;
		}
	}
}

void check_general_optarg(char *optarg, char option)
{
	if (option == 's' || option == 'w') {
		master_current = option;
	}

	if(optarg != NULL) {
			if(optarg[0] != '=') {
				print_error(1);
				exit(1);
			}
			if(strlen(optarg) == 1) {
				print_error(2);
				exit(1);
			}
		}

	switch (option) {
		case 's': {
			if (optarg == NULL) {
				print_error(6);
				exit(1);
			}
		}
	}

}

void check_int_semicolon_optarg(char *optarg, char master_current, char option)
{
	once_or_twice = 0;
	if (master_current == 's') {
	for (int i = 1, n = strlen(optarg); i < n; i = i + 1) {
			if (!isdigit(optarg[i]) && optarg[i] != ':') {
				print_error(3);
				exit(1);
			}
			else if(optarg[i] == ':') {
				if(once_or_twice > 2) {
					print_error(4);
					exit(1);
				}

				parse[once_or_twice] = i;
				once_or_twice = once_or_twice + 1;
				optarg[i] == '\0';
			}
		}
		if (once_or_twice == 1) {
			if (option == 'c') {
				split.column_l = atoi(&optarg[1]);
				split.column_r = atoi(&optarg[parse[0] + 1]);
			}
			else if (option == 'r') {
				split.row_l = atoi(&optarg[1]);
				split.row_r = atoi(&optarg[parse[0] + 1]);
			}
		}
		else if (once_or_twice == 2) {
			if (option == 'r') {
				split.row_l = atoi(&optarg[1]);
				split.row_m = atoi(&optarg[parse[0] + 1]);
				split.row_r = atoi(&optarg[parse[1] + 1]);
			}
			else if (option == 'c') {
				split.column_l = atoi(&optarg[1]);
				split.column_m = atoi(&optarg[parse[0] + 1]);
				split.column_r = atoi(&optarg[parse[1] + 1]);
			}
		}
		else {
			print_error(14);
			exit(1);
		}
	}
	else if(option == 'c') {
		columns = atoi(&optarg[1]);
	}
	else if(option == 'r') {
		rows = atoi(&optarg[1]);
	}
}

void check_master_current(char master_current)
{
	if (master_current == 0) {
		print_error(12);
		exit(1);
	}
}

void check_columns(char *optarg, char master_current, char option)
{
	check_master_current(master_current);

	check_general_optarg(optarg, option);

	check_int_semicolon_optarg(optarg, master_current, option);
}

void check_rows(char *optarg, char master_current, char option)
{
	check_master_current(master_current);

	check_general_optarg(optarg, option);

	check_int_semicolon_optarg(optarg, master_current, option);
}

void excess_arguments(int optind, int argc)
{
	while(optind < argc) {
		optind = optind + 1;
	}
}

void check_read_OR_write(flags flag)
{
	if(flag.w_flag == 1 && flag.s_flag == 1) {
		print_error(5);
		exit(1);
	}
}

void check_file(FILE *inptr)
{
	if (inptr == NULL) {
		print_error(8);
		free(inptr);
		exit(1);
	}
}

void check_memory(char *alloced_char)
{
	if (alloced_char == NULL) {
		print_error(11);
		free(alloced_char);
		exit(1);
	}
}

void set_up_write(flags flag, char *input_file_name, char *default_file_name, char current)
{
	if (current == 'w') {
		if(flag.w_flag == 1 && flag.r_flag == 1 && flag.c_flag == 1) {
			if (input_file_name != NULL) {
				inptr = fopen(input_file_name, "w");

				check_file(inptr);
			}
			else {
				 inptr = fopen(default_file_name, "w");

				 check_file(inptr);
			}
			if (once_or_twice == 1) {
				print_error(9);
				exit(1);
			}
		}
		else {
			print_error(7);
			exit(1);
		}
	}
}

void implement_write(int columns, int rows, FILE **inptr, char current)
{		
	if (current == 'w') {
		buffer = malloc(sizeof(char) * 20);
		char *arr_column[rows][columns];

		for (int i = 0; i < rows; i = i + 1) {
			for (int j = 0; j < columns; j = j + 1) {
				arr_column[i][j] = malloc(sizeof(char) * 20);
				check_memory(arr_column[i][j]);
			}
		}

		for(int i = 0, n = columns; i < n; i = i + 1) {
			printf("COLUMN name %i: ", i);
			scanf("%0" STR(MAX) "s", buffer);
			fputs(buffer, *inptr);
			fputc(',', *inptr);

			strcpy(arr_column[0][i], buffer);
		}

		fputc('\n', *inptr);

		for(int i = 0, k = rows; i < k; i = i + 1) {
			for(int j = 0, n = columns; j < n; j = j + 1) {
				printf("Value %i %s: ", i + 1, arr_column[0][j]);
				scanf("%0" STR(MAX) "s", buffer);
				fputs(buffer, *inptr);
				putc(',', *inptr);
			}

			fputc('\n', *inptr);
		}

		free(buffer);

		for(int i = 0; i < rows; i = i + 1) {
			for(int j = 0; j < columns; j = j + 1) {
				free(arr_column[i][j]);
			}
		}	

		fclose(*inptr);
		exit(0);
	}
}

void set_up_scan(flags flag, char *input_file_name, char current)
{
	if (current == 's') {
		if (flag.s_flag == 1 && flag.r_flag == 1 && flag.c_flag == 1) {
			inptr = fopen(input_file_name, "r");
			
			check_file(inptr);
		}
		else {
			print_error(10);
			exit(1);
		}
		
		if (split.row_l > split.row_r || split.column_l > split.column_r) {
			print_error(13);
			exit(1);
		}
	}
}

node ** read_file(FILE **inptr, node **root, int column_l, int column_r, int row_l, int row_r)
{
	buffer = malloc(sizeof(char) * 50);
	tmp = 0;
	for (int i = 0, n = row_r; i < n; i = i + 1) {
		node_iteration = 0;
		for (int j = 0, m = column_r; j < m; j = j + 1) {
			tmp = 0;
			c = 0;
			count = 0;
			while(c != ',') {
				count = count + 1;
				if (count > 50) {
					print_error(16);
					exit(1);
				}
				fread(&c, sizeof(char), 1, *inptr);
				buffer[tmp] = c;
				tmp = tmp + 1;
			}
			buffer[tmp - 1] = '\0';

			if (i >= row_l - 1 && j >= column_l - 1) {
				if (!atoi(buffer)) {
					print_error(15);
					exit(1);
				}
				node *new_node = malloc(sizeof(node));
				new_node->input = atoi(buffer);
				new_node->next = root[node_iteration];
				root[node_iteration] = new_node;
				if (column_l != column_r && row_l != row_r) {
					node_iteration = node_iteration + 1;
				}
			}
		}
		while(tmp = fgetc(*inptr) != 10); /*Jump to next Line*/
	}
	free(buffer);
	fclose(*inptr);
	return root;
}

node ** implement_scan(splits split, flags flag, FILE **inptr, int master_current)
{
	if (once_or_twice == 2) {

		node **root = malloc(sizeof(node) * split.column_m);
		array_count = split.column_m - split.column_l + 1;
		for (int i = 0; i < array_count; i = i + 1) {
		root[i] = malloc(sizeof(node));
		root[i]->input = 0;
		root[i]->next = NULL;
		}
		return read_file(inptr, root, split.column_l, split.column_m, split.row_l, split.row_m);
	}
	
	if (once_or_twice == 1); {
		node **root;

		return read_file(inptr, root, split.column_l, split.column_l, split.row_l, split.row_l);
	}
 
}

void free_node(node *root)
{
	if(root == NULL)
	{
		return;
	}
	else
	{
		free_node(root->next);
	}
	free(root);
}
