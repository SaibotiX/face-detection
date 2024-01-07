#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	if(argc != 3 || !atoi(argv[1]) || !atoi(argv[2]))
	{
		printf("./a.out [columns] [rows]\n");
		return 1;
	}

	FILE *csv = fopen("document.csv", "r+");
	if(csv == NULL)
	{
		printf("File not found");
		return 1;
	}

	char *buffer = malloc(sizeof(char) * 20);
	for(int i = 0, n = atoi(argv[1]); i < n; i = i + 1)
	{
		printf("COLUMN name: ");
		scanf("%19s", buffer);
		fputs(buffer, csv);
		fputc(',', csv);
	}
	fclose(csv);
}
