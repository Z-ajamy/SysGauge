#include <stdio.h>
#include <stdlib.h>
#include "../include/sysgauge.h"

/**
 * main - Entry point for testing the core engine
 *
 * Description: Hardcodes commands to test the run_engine function directly
 * and prints the raw structural metrics to the standard output.
 *
 * Return: Always 0 (Success)
 */
int main(void)
{
	program_data_t test1;
	char *args1[] = {"sleep", "1", NULL};

	program_data_t test2;
	char *args2[] = {"ls", NULL};

	program_data_t test3;
	char *args3[] = {"fake_program", NULL};

	printf("--- SysGauge Core Engine Unit Test ---\n\n");

	/* Test 1: Verify time measurement using 'sleep' */
	test1.program_path = "sleep";
	test1.args = args1;
	printf("[Test 1] Executing: sleep 1\n");
	run_engine(&test1);
	printf("-> Status code: %d (Expected: %d for SUCCESS)\n", test1.status, SUCCESS);
	printf("-> Execution time: %.3f ms (Expected: ~1000 ms)\n", test1.time_ms);
	printf("-> Max memory (KB): %ld\n\n", test1.memory_kb);

	/* Test 2: Verify normal execution and memory footprint using 'ls' */
	test2.program_path = "ls";
	test2.args = args2;
	printf("[Test 2] Executing: ls\n");
	run_engine(&test2);
	printf("-> Status code: %d (Expected: %d for SUCCESS)\n", test2.status, SUCCESS);
	printf("-> Execution time: %.3f ms\n", test2.time_ms);
	printf("-> Max memory (KB): %ld\n\n", test2.memory_kb);

	/* Test 3: Verify execution failure handling */
	test3.program_path = "fake_program";
	test3.args = args3;
	printf("[Test 3] Executing: fake_program\n");
	run_engine(&test3);
	printf("-> Status code: %d (Expected: %d for EXEC_FAILED)\n", test3.status, EXEC_FAILED);
	printf("-> Execution time: %.3f ms\n", test3.time_ms);
	printf("-> Max memory (KB): %ld\n\n", test3.memory_kb);

	return (0);
}
