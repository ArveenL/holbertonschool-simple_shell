#include <stdio.h>   /* printf, perror */
#include <stdlib.h>  /* exit, free */
#include <unistd.h>  /* fork, execve */
#include <string.h>  /* strlen */
#include <sys/wait.h> /* waitpid */

/**
 * main - simple UNIX command line interpreter
 *
 * Return: 0 on success
 */
int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;

	while (1) /* infinite loop → shell stays alive */
	{
		pid_t child_pid;
		int status;
		char *argv[2];
		extern char **environ;

		printf("$ "); /* display prompt */
		nread = getline(&line, &len, stdin); /* read input from user */

		if (nread == -1) /* EOF (Ctrl+D) */
		{
			printf("\n");
			break; /* exit shell loop */
		}

		/* remove newline at the end */
		if (line[nread - 1] == '\n')
			line[nread - 1] = '\0';

		if (line[0] == '\0') /* empty command, just continue */
			continue;

		child_pid = fork(); /* create child process */
		if (child_pid == -1) /* fork error */
		{
			perror("fork");
			continue;
		}

		if (child_pid == 0) /* child process */
		{
			argv[0] = line;
			argv[1] = NULL;

			execve(line, argv, environ); /* execute command */

			/* if execve returns, there was an error */
			perror("Error");
			exit(EXIT_FAILURE);
		}
		else /* parent process */
		{
			waitpid(child_pid, &status, 0); /* wait for child to finish */
		}
	}

	free(line);
	return (0);
}

