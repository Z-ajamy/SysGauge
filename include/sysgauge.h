#ifndef SYSGAUGE_H
#define SYSGAUGE_H

#include <sys/types.h>

/**
 * enum exec_status - Represents the execution status of a program
 * @SUCCESS: Program executed and exited normally
 * @EXEC_FAILED: System failed to start the program executable
 * @SEGFAULT: Program crashed due to memory access violation
 * @TIMEOUT: Program was forcibly killed after exceeding time limit
 * @UNKNOWN_ERR: Program terminated for an unhandled reason
 */
typedef enum exec_status
{
	SUCCESS,
	EXEC_FAILED,
	SEGFAULT,
	TIMEOUT,
	UNKNOWN_ERR
} status_t;

/**
 * struct program_data - Holds input arguments and output metrics for a program
 * @program_path: Path or name of the executable
 * @args: Null-terminated array of arguments for execvp
 * @status: Final execution status of the program
 * @time_ms: Average real execution time in milliseconds
 * @memory_kb: Maximum resident set size (memory footprint) in KB
 */
typedef struct program_data
{
	char *program_path;
	char **args;
	status_t status;
	double time_ms;
	long memory_kb;
} program_data_t;

/* Function Prototypes */

/**
 * run_engine - Executes a program in an isolated process and measures metrics
 * @data: Pointer to the program_data_t structure to read args and write results
 *
 * Description: Forks a new process, executes the target program, measures
 * wall-clock time, tracks memory footprint, and handles execution errors.
 *
 * Return: 0 on successful measurement, -1 on internal system failure (e.g. fork)
 */
int run_engine(program_data_t *data);
char **parse_command(char *cmd);


/* Interface Prototypes */
program_data_t *prepare_workload(int argc, char **argv, int *workload_count);
void free_workload(program_data_t *workload, int count);
void print_report(program_data_t *workload, int count);
void sort_results(program_data_t *workload, int count);



#endif
