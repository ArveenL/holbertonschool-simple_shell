All exercises/tasks that were given to be done before attacking the main shell project

------------------------

task 0

#include <stdio.h>
#include <unistd.h>					//unistd is used for getppid()

int main(void)
{
        pid_t parent_pid = getppid();			//getppid() is a system function in linux and it returns the PID of the parent process
							//pid_t is a data type used to store process ID

        printf("Parent PID: %d\n", parent_pid);		//Print the parent PID to the terminal

        return 0;
}

------------------------

task 1
