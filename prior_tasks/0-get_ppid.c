#include <stdio.h>
#include <unistd.h>

int main (void)
{
	pid_t parent_pid = getppid();

	printf("Parent PID: %d\n", parent_pid);

	return 0;
}
