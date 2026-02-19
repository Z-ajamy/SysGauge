#include <stdio.h>
#include <stdlib.h>
#include "include/sysgauge.h"

/**
 * main - Entry point for the SysGauge benchmarking tool
 * @argc: Argument count
 * @argv: Argument vector
 *
 * Return: 0 on success, 1 on error
 */
int main(int argc, char **argv)
{
	program_data_t *workload;
	int count = 0;
	int i;

	/* 1. Validate Input */
	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s \"command1 args\" \"command2 args\" ...\n", argv[0]);
		return (1);
	}

	/* 2. Prepare Workload (Interface Layer) */
	workload = prepare_workload(argc, argv, &count);
	if (!workload)
	{
		fprintf(stderr, "Error: Failed to initialize workload.\n");
		return (1);
	}

	printf("SysGauge: Benchmarking %d program(s) sequentially...\n", count);

	/* 3. Execute and Measure (Core Engine Layer) */
	for (i = 0; i < count; i++)
	{
		if (workload[i].program_path != NULL)
		{
			run_engine(&workload[i]);
		}
	}
    for (i = 0; i < count; i++)
	{
		if (workload[i].program_path != NULL)
		{
			run_engine(&workload[i]);
		}
	}

	/* Sort the results before displaying */
	sort_results(workload, count);

	/* 4. Display Results (Reporting Layer) */
	print_report(workload, count);

	/* 5. Clean Up Memory */
	free_workload(workload, count);

	return (0);
}
