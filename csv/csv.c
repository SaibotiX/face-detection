#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int csv_write(char **argv, FILE **csv, int columns);
void create_helper(int file_char_count, FILE **helper);
void csv_helper_read(FILE **csv, FILE **helper);

int main(int argc, char *argv[])
{
	FILE *csv, *helper;
	int file_char_count = 0;
	if(argc == 2 && strlen(argv[1]) == 1 && argv[1][0] == 114)
	{
		csv = fopen("document.csv", "r");
		helper = fopen("helper.txt", "r");
		if(csv == NULL || helper == NULL)
		{
			printf("File not found\n");
			return 1;
		}

		csv_helper_read(&csv, &helper);

		fclose(csv);
		fclose(helper);
		return 0;
	}
	else if(argc == 4 && atoi(argv[1]) && atoi(argv[2]) && strlen(argv[3]) == 1 && argv[3][0] == 119)
	{
		csv = fopen("document.csv", "w");
		
		if(csv == NULL)
		{
			printf("File not found\n");
			return 1;
		}

		file_char_count = csv_write(&argv[0], &csv, atoi(argv[1]));
		
		create_helper(file_char_count, &helper);
		
		fclose(csv);
		return 0;
	}
	
	printf("./a.out r (For reading .csv file)\n");
	printf("./a.out [columns] [rows] w (For writing .csv file)\n");
	return 1;
}

int csv_write(char **argv, FILE **csv, int columns)
{
	char *buffer = malloc(sizeof(char) * 20);
	char *arr_column[atoi(argv[1])];
	int char_count = 0;

	for(int i = 0, n = atoi(argv[1]); i < n; i = i + 1)
	{
		printf("COLUMN name: ");
		scanf("%19s", buffer);
		fputs(buffer, *csv);
		fputc(',', *csv);

		arr_column[i] = malloc(sizeof(char) * (strlen(buffer) + 1));
		strcpy(arr_column[i], buffer);

		char_count = char_count + strlen(buffer) + 1;
	}
	
	fputc('\n', *csv);
	char_count = char_count + 1;

	for(int j = 0, m = atoi(argv[1]); j < m; j = j + 1)
	{
		for(int i = 0, n = atoi(argv[2]); i < n; i = i + 1)
		{
			printf("%i Value for %s ", j + 1, arr_column[i]);
			scanf("%19s", buffer);
			fputs(buffer, *csv);
			putc(',', *csv);

			char_count = char_count + strlen(buffer) + 1;
		}
		fputc('\n', *csv);
		char_count = char_count + 1;
	}

	free(buffer);
	
	for(int i = 0; i < columns; i = i + 1)
	{
		free(arr_column[i]);
	}	
	return char_count;
}

void csv_helper_read(FILE **csv, FILE **helper)
{
	char *tmp = malloc(sizeof(char) * 8), *buffer;
	int char_number;
	
	fscanf(*helper, "%s", tmp);
	char_number = atoi(tmp);
	
	buffer = malloc(sizeof(char) * (char_number + 1));
	fread(buffer, char_number, 1, *csv);

	buffer[char_number] = '\0';
	
	printf("%s", buffer);

	free(tmp);
	free(buffer);
	return;
}

void create_helper(int file_char_count, FILE **helper)
{
	*helper = fopen("helper.txt", "w");
	fprintf(*helper, "%i", file_char_count);
	fputc('\n', *helper);
	fclose(*helper);
}

