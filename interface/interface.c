#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../include/sysgauge.h"

/**
 * parse_command - Splits a command string into an array of arguments
 * @cmd: The command string (e.g., "ls -l -a")
 *
 * Description: Modifies the original string by replacing spaces with null
 * terminators using strtok, and populates an array of pointers.
 *
 * Return: A dynamically allocated NULL-terminated array of strings,
 * or NULL if memory allocation fails.
 */
char **parse_command(char *cmd)
{
	char **args;
	char *token;
	int i = 0;

	/* Allocate memory for up to 64 arguments (including the NULL terminator) */
	args = malloc(sizeof(char *) * 64);
	if (!args)
		return (NULL);

	/* Split the string by spaces and tabs */
	token = strtok(cmd, " \t");
	while (token != NULL && i < 63)
	{
		args[i] = token;
		i++;
		token = strtok(NULL, " \t");
	}
	args[i] = NULL;

	return (args);
}

/**
 * prepare_workload - Parses CLI arguments into an array of program_data_t
 * @argc: Argument count from main
 * @argv: Argument vector from main
 * @workload_count: Pointer to store the number of valid programs prepared
 *
 * Description: Iterates over the raw CLI strings, allocates the data
 * structures, and prepares them for the core engine.
 *
 * Return: Array of program_data_t, or NULL on failure.
 */
program_data_t *prepare_workload(int argc, char **argv, int *workload_count)
{
	program_data_t *workload;
	int i, num_programs;

	/* The first argument is the benchmarker itself, so we subtract 1 */
	num_programs = argc - 1;
	if (num_programs <= 0)
	{
		*workload_count = 0;
		return (NULL);
	}

	workload = malloc(sizeof(program_data_t) * num_programs);
	if (!workload)
		return (NULL);

	for (i = 0; i < num_programs; i++)
	{
		/* argv[i + 1] contains the command string like "ls -l" */
		workload[i].args = parse_command(argv[i + 1]);
		
		if (workload[i].args != NULL && workload[i].args[0] != NULL)
		{
			/* The first token is the program path/name */
			workload[i].program_path = workload[i].args[0];
		}
		else
		{
			workload[i].program_path = NULL;
		}
		
		/* Initialize default metrics */
		workload[i].status = UNKNOWN_ERR;
		workload[i].time_ms = 0.0;
		workload[i].memory_kb = 0;
	}

	*workload_count = num_programs;
	return (workload);
}

/**
 * free_workload - Frees the dynamically allocated workload structures
 * @workload: Array of program_data_t to free
 * @count: Number of elements in the array
 *
 * Return: void
 */
void free_workload(program_data_t *workload, int count)
{
	int i;

	if (!workload)
		return;

	for (i = 0; i < count; i++)
	{
		if (workload[i].args)
			free(workload[i].args);
	}
	free(workload);
}
