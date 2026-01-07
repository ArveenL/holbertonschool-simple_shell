#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

/**
 * main - creates 5 child processes, each executing ls -l /tmp
 *
 * Return: 0 on success
 */


int main(void)
{
    pid_t pid;
    int i;
    char *args[] = {"/bin/ls", "-l", "/tmp", NULL};

    for (i = 0; i < 5; i++)
    {
        pid = fork();
        if (pid == 0)
        {
            execve("/bin/ls", args, NULL);
            exit(1); /* only if execve fails */
        }
        wait(NULL); /* parent waits for child */
    }

    return 0;
}

