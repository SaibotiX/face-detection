#include "csv.h"
#include <time.h>

int main(int argc, char *argv[])
{
	result csv = csv_function(argc, argv);
	
	/* print the input */
	for(int i = 0; i < array_count; i = i + 1) {
		node *ptr = csv.input[i];
		while(ptr->next != NULL) {
			printf("%i ", ptr->input);
			ptr = ptr->next;
		}
		printf("\n");
	}
	
	/* print the results */
	for(int i = 0; i < 1; i = i + 1) {
		node *ptr = csv.output[i];
		while(ptr ->next != NULL) {
			printf("%i ", ptr->input);
			ptr = ptr->next;
		}
		printf("\n");
	}

	call_free_functions(csv, buffer);

	return 0;
}
