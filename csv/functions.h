#define _STR(x) #x
#define STR(x) _STR(x)

#define MAX 20

FILE *inptr = NULL;
char *buffer;
int array_count = 0;

/* Tracking and then verifying which Arguments the user used */
typedef struct {
	int w_flag;
	int s_flag;
	int c_flag;
	int r_flag;
	int k_flag;
} flags;

flags flag;

/* This struct is used to decompose the -r= and -c= values like: 12:14:15 */
typedef struct {
	int column_l;
	int column_m;
	int column_r;

	int row_l;
	int row_m;
	int row_r;
} splits;

typedef struct {
	int column;
	int row;
}layout;

/* This node gets used to store the inputs and the output value of the csv file */
typedef struct node {
	int input;
	struct node *next;
} node;

/* This type gets returned in order to use the result elsewhere */
typedef struct {
	node **input;
	node **output;
} result;

/* This is needed for the getopt() function */
static struct option long_options[] = {
	{"write", optional_argument, &flag.w_flag, 1},
	{"scan", required_argument, &flag.s_flag, 1},
	{"column", required_argument, &flag.c_flag, 1},
	{"rows", required_argument, &flag.r_flag, 1},
	{0,0,0,0}
};

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
			printf("You gotta provide 2 : for [from input 1]:[to input 2]:[output] (This is handy for machine learning where you have multiple inputs but only one output\n");
			break;
		}
		case 15: {
			printf("Make sure to read only integers\n");
			break;
		}
		case 16: {
			printf("Make sure you have no words longer than 49 Characters and that you didnt surpass the end of the file\n");
			break;
		}
		case 17: {	
			printf("Invalid Argument\n");
			break;
		}
		case 18: {
			printf("Make sure your Arguments are set correctly. Numbers for -c -r for write only!\n");
			break;
		}
		case 19: {
			printf("Make sure your columns and rows are from lowest to highest.\n");
			break;
		}
		case 69: {
			/* Nothing happens, except the first notification */
			break;
		}
	}
	exit(1);
}

void check_general_optarg(char *optarg, char identifier)
{
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
	else if(identifier != 's' && optarg == NULL)
	{
		print_error(6);
		exit(1);
	}
}

void initialize_master_current(char *master_current, char c)
{
	if(*master_current == 0) {
		*master_current = c;
	}
	else {
		print_error(5);
		exit(1);
	}
}

/* Make it so the scan and the write uses different functions because it does not work like I intendet! */

int * track_semicolons(void)
{
	int twice = 0;
	int *position_semicolon = malloc(sizeof(int) * 2);
	position_semicolon[0] = 0;
	position_semicolon[1] = 0;

	for (int i = 1, n = strlen(optarg); i < n; i = i + 1) {
		if (!isdigit(optarg[i]) && optarg[i] != ':') {
			print_error(3);
			exit(1);
		}
		else if(optarg[i] == ':') {
			if(twice > 2) {
				print_error(14);
				exit(1);
			}

			position_semicolon[twice] = i;
			twice = twice + 1;
			optarg[i] == '\0';
		}
	}
	return position_semicolon;
}

void define_split(char *optarg, splits *split, char identifier, int *position_semicolons)
{
	if(position_semicolons[0] == 0 || position_semicolons[1] == 0) {
		print_error(14);
		exit(1);
	}

	if (identifier == 'r') {
		split->row_l = atoi(&optarg[1]);
		split->row_m = atoi(&optarg[position_semicolons[0] + 1]);
		split->row_r = atoi(&optarg[position_semicolons[1] + 1]);
	}
	else if (identifier == 'c') {
		split->column_l = atoi(&optarg[1]);
		split->column_m = atoi(&optarg[position_semicolons[0] + 1]);
		split->column_r = atoi(&optarg[position_semicolons[1] + 1]);
	}
	free(position_semicolons);
}

void define_table(layout *table, char identifier)
{
	if(identifier == 'c') {
		if (!atoi(&optarg[1])) {
			print_error(18);
			exit(1);
		}
		table->column = atoi(&optarg[1]);
	}
	else if(identifier == 'r') {
		if (!atoi(&optarg[1])) {
				print_error(18);
				exit(1);
		}
		table->row = atoi(&optarg[1]);
	}
}

void excess_arguments(int optind, int argc)
{
	if(optind < argc) {
		print_error(17);
		exit(1);
	}
}

/* ------------------------------------------------------- */
void check_master_current(char master_current)
{
	if (master_current == 0) {
		print_error(12);
		exit(1);
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

void check_split(splits split)
{
	if (split.column_l > split.column_m || split.column_l > split.column_r || split.column_m > split.column_r) {
		print_error(19);
		exit(1);
	}
	else if(split.row_l > split.row_m || split.row_m > split.row_r || split.row_m > split.row_r) {
		print_error(19);
		exit(1);
	}
}
/* ----------------------------------------------------------*/

/* ---------------------------------------------------------------*/
void free_node(node *root)
{
	if(root == NULL) {
		return;
	}
	else {
		free_node(root->next);
	}
	free(root);
}

void free_input(node **input)
{
	for(int i = 0; i < array_count; i = i + 1) {
		free_node(input[i]);
	}
	free(input);
}

void free_output(node **output)
{
	free_node(output[0]);
	free(output);
}

void free_everything_else(FILE *inptr,char *buffer)
{
	free(buffer);
	fclose(inptr);
}

void call_free_functions(result csv, char *buffer)
{
	free_input(csv.input);
	free_output(csv.output);
	free_everything_else(inptr, buffer);
}
/* ----------------------------------------------------------------- */

/* ----------------------------------------------------------------------------------------------------- */
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
		}
		else {
			print_error(7);
			exit(1);
		}
	}
}

void implement_write(layout table, char master_current)
{		
	if (master_current == 'w') {
		char *buffer = malloc(sizeof(char) * 20);
		char *arr_column[table.row][table.column];

		for (int i = 0; i < table.row; i = i + 1) {
			for (int j = 0; j < table.column; j = j + 1) {
				arr_column[i][j] = malloc(sizeof(char) * 20);
				check_memory(arr_column[i][j]);
			}
		}

		for(int i = 0, n = table.column; i < n; i = i + 1) {
			printf("COLUMN name %i: ", i);
			scanf("%0" STR(MAX) "s", buffer);
			fputs(buffer, inptr);
			fputc(',', inptr);

			strcpy(arr_column[0][i], buffer);
		}

		fputc('\n', inptr);

		for(int i = 0, k = table.row; i < k; i = i + 1) {
			for(int j = 0, n = table.column; j < n; j = j + 1) {
				printf("Value %i %s: ", i + 1, arr_column[0][j]);
				scanf("%0" STR(MAX) "s", buffer);
				fputs(buffer, inptr);
				putc(',', inptr);
			}

			fputc('\n', inptr);
		}

		free(buffer);

		for(int i = 0; i < table.row; i = i + 1) {
			for(int j = 0; j < table.column; j = j + 1) {
				free(arr_column[i][j]);
			}
		}	

		fclose(inptr);
		exit(0);
	}
}
/* ---------------------------------------------------------------------------------------------------------- */


void set_up_scan(flags flag, char *input_file_name, char master_current)
{
	if (master_current == 's') {
		if (flag.s_flag == 1 && flag.r_flag == 1 && flag.c_flag == 1) {
			inptr = fopen(input_file_name, "r");
			
			check_file(inptr);
		}
		else {
			print_error(10);
			exit(1);
		}
	}
}

void read_char(result csv, char *buffer)
{
	int tmp = 0, c = 0, count = 0;

	while(c != ',') {
		count = count + 1;
		if (count > 50) {
			print_error(16);

			call_free_functions(csv, buffer);
			exit(1);
		}
		fread(&c, sizeof(char), 1, inptr);
		buffer[tmp] = c;
		tmp = tmp + 1;
	}
	buffer[tmp - 1] = '\0';
}

void check_input_value(char * buffer, result csv)
{
	if (!atoi(buffer)) {
		call_free_functions(csv, buffer);

		print_error(15);
		exit(1);
	}
}

void create_nodes(node **linked_list, result csv, char *buffer, int node_iteration)
{
	check_input_value(buffer, csv);

	node *new_node = malloc(sizeof(node));
	new_node->input = atoi(buffer);
	new_node->next = linked_list[node_iteration];
	linked_list[node_iteration] = new_node;
}

void read_file_input_output(result csv, splits split)
{
	int tmp = 0;
	int node_iteration = 0;
	buffer = malloc(sizeof(char) * 50);
	for (int i = 0, n = split.row_m; i < n; i = i + 1) {
		node_iteration = 0;
		for (int j = 0, m = split.column_m; j < m; j = j + 1) {
			read_char(csv, buffer);

			if (i >= split.row_l - 1 && j >= split.column_l - 1) {
				create_nodes(csv.input, csv, buffer, node_iteration);
				node_iteration = node_iteration + 1; 
			}
		}

		node_iteration = 0;

		for(int l = 0, p = split.column_r - split.column_m; l < p; l = l + 1) {
			read_char(csv, buffer);

			if(l == p - 1 && i >= split.row_l - 1) {
				create_nodes(csv.output, csv, buffer, 0);
			}
		}
		while(tmp = fgetc(inptr) != 10); /*Jump to next Line*/

	}
}

result initialize_result(splits split)
{
	result csv;
	csv.output = malloc(sizeof(node));
	csv.output[0] = malloc(sizeof(node));
	csv.output[0]->input = 0;
	csv.output[0]->next = NULL;

	csv.input = malloc(sizeof(node) * array_count);
	for (int i = 0; i < array_count; i = i + 1) {
		csv.input[i] = malloc(sizeof(node));
		csv.input[i]->input = 0;
		csv.input[i]->next = NULL;
	}
	return csv;
}

result implement_scan(splits split, char master_current)
{
	if (master_current = 's') {
		array_count = split.column_m - split.column_l + 1;

		result csv = initialize_result(split);

		read_file_input_output(csv, split); 
		
		return csv;
	}
}

