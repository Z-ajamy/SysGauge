#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <time.h>
#include <stdlib.h>
#include <fcntl.h>
#include "../include/sysgauge.h"

int run_engine(program_data_t *data) {
    pid_t pid;
    int status;
    struct timespec start, end;
    struct rusage useage;
    double start_ms, end_ms;

    clock_gettime(CLOCK_MONOTONIC, &start);
    pid = fork();

    if (pid < 0)
        return(-1);
    
    if (pid == 0)
	{
		int dev_null;

		/* Redirect standard output and error to /dev/null */
		dev_null = open("/dev/null", O_WRONLY);
		if (dev_null != -1)
		{
			dup2(dev_null, STDOUT_FILENO);
			dup2(dev_null, STDERR_FILENO);
			close(dev_null);
		}

		/* Child process: replace current image with the target program */
		execvp(data->program_path, data->args);
		
		/* If execvp returns, execution failed */
		exit(127);
	}
    else
    {
        wait4(pid, &status, 0, &useage);
        clock_gettime(CLOCK_MONOTONIC, &end);
        start_ms = (start.tv_sec * 1000.0) + (start.tv_nsec / 1000000.0);
		end_ms = (end.tv_sec * 1000.0) + (end.tv_nsec / 1000000.0);
		data->time_ms = end_ms - start_ms;
        data->memory_kb = useage.ru_maxrss;

        if (WIFEXITED(status))
		{
			if (WEXITSTATUS(status) == 127)
				data->status = EXEC_FAILED;
			else
				data->status = SUCCESS;
		}
		else if (WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGSEGV)
				data->status = SEGFAULT;
			else if (WTERMSIG(status) == SIGKILL)
				data->status = TIMEOUT;
			else
				data->status = UNKNOWN_ERR;
		}
    }
    return 0;
}
