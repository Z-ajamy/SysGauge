#include <stdio.h>
#include <stdlib.h>
#include "../include/sysgauge.h"

/**
 * main - Entry point for testing the interface layer
 *
 * Description: Simulates command-line arguments to test the string parsing
 * and memory allocation logic in prepare_workload.
 *
 * Return: Always 0 (Success)
 */
int main(void)
{
	char prog_name[] = "./sysgauge";
	char cmd1[] = "ls -l -a";
	char cmd2[] = "sleep 1";
	char cmd3[] = "echo Hello World";
	
	/* Declare the array first, then assign elements to satisfy C89 rules */
	char *fake_argv[5];
	int fake_argc = 4;
	
	int count = 0;
	program_data_t *workload;
	int i, j;

	fake_argv[0] = prog_name;
	fake_argv[1] = cmd1;
	fake_argv[2] = cmd2;
	fake_argv[3] = cmd3;
	fake_argv[4] = NULL;

	printf("--- SysGauge Interface Unit Test ---\n\n");

	workload = prepare_workload(fake_argc, fake_argv, &count);

	if (!workload)
	{
		printf("Error: Failed to prepare workload.\n");
		return (1);
	}

	printf("Successfully parsed %d programs.\n\n", count);

	for (i = 0; i < count; i++)
	{
		printf("[Program %d] Path: '%s'\n", i + 1, workload[i].program_path);
		printf("Parsed Arguments:\n");
		
		for (j = 0; workload[i].args != NULL && workload[i].args[j] != NULL; j++)
		{
			printf("  arg[%d]: '%s'\n", j, workload[i].args[j]);
		}
		printf("\n");
	}

	free_workload(workload, count);
	printf("Memory freed successfully. Interface test passed!\n");

	return (0);
}
