#include <stdio.h>
#include "../include/sysgauge.h"

#include <stdio.h>
#include <stdlib.h>
#include "../include/sysgauge.h"

/**
 * compare_metrics - Comparison function for qsort
 * @a: Pointer to first program_data_t element
 * @b: Pointer to second program_data_t element
 *
 * Description: Sorts by execution status (SUCCESS first),
 * then by execution time in ascending order (fastest first).
 *
 * Return: -1 if a goes before b, 1 if a goes after b, 0 if equal
 */
int compare_metrics(const void *a, const void *b)
{
	program_data_t *prog_a = (program_data_t *)a;
	program_data_t *prog_b = (program_data_t *)b;

	/* Prioritize successful executions over failures */
	if (prog_a->status == SUCCESS && prog_b->status != SUCCESS)
		return (-1);
	if (prog_a->status != SUCCESS && prog_b->status == SUCCESS)
		return (1);

	/* Sort by execution time (fastest to slowest) */
	if (prog_a->time_ms < prog_b->time_ms)
		return (-1);
	if (prog_a->time_ms > prog_b->time_ms)
		return (1);

	return (0);
}

/**
 * sort_results - Sorts the workload array based on performance
 * @workload: Array of program_data_t containing the results
 * @count: Number of programs in the workload
 *
 * Return: void
 */
void sort_results(program_data_t *workload, int count)
{
	qsort(workload, count, sizeof(program_data_t), compare_metrics);
}


/**
 * print_report - Outputs the final benchmarking report
 * @workload: Array of program_data_t containing the results
 * @count: Number of programs in the workload
 *
 * Description: Prints a formatted table with execution status,
 * time taken, and maximum memory footprint.
 *
 * Return: void
 */
void print_report(program_data_t *workload, int count)
{
	int i;
	char *status_str;

	printf("\n=================================================================\n");
	printf("%-20s | %-12s | %-12s | %-10s\n", "Program", "Status", "Time (ms)", "Max Mem (KB)");
	printf("=================================================================\n");

	for (i = 0; i < count; i++)
	{
		switch (workload[i].status)
		{
			case SUCCESS:
				status_str = "SUCCESS";
				break;
			case EXEC_FAILED:
				status_str = "EXEC_FAILED";
				break;
			case SEGFAULT:
				status_str = "SEGFAULT";
				break;
			case TIMEOUT:
				status_str = "TIMEOUT";
				break;
			default:
				status_str = "UNKNOWN_ERR";
				break;
		}

		printf("%-20.20s | %-12s | %-12.3f | %-10ld\n",
		       workload[i].program_path ? workload[i].program_path : "NULL",
		       status_str,
		       workload[i].time_ms,
		       workload[i].memory_kb);
	}
	printf("=================================================================\n");
}
