#include <stdio.h>   /* printf, perror */
#include <stdlib.h>  /* exit, malloc, free */
#include <unistd.h>  /* fork, execve, access */
#include <sys/wait.h> /* wait */
#include <string.h>  /* strlen */

/**
 * main - A simple UNIX command line interpreter
 *
 * Return: Always 0
 */
int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;
	pid_t child_pid;
	int status;
	char *argv[2];

	while (1)
	{
		/* Display prompt */
		printf("$ ");
		fflush(stdout);

		/* Read input from user */
		nread = getline(&line, &len, stdin);
		if (nread == -1) /* EOF (Ctrl+D) */
		{
			printf("\n");
			break;
		}

		/* Remove newline */
		if (line[nread - 1] == '\n')
			line[nread - 1] = '\0';

		/* Skip empty lines */
		if (line[0] == '\0')
			continue;

		argv[0] = line;
		argv[1] = NULL;

		child_pid = fork();
		if (child_pid == -1)
		{
			perror("fork");
			free(line);
			exit(EXIT_FAILURE);
		}

		if (child_pid == 0) /* Child process */
		{
			if (access(line, X_OK) == 0)
				execve(line, argv, environ);

			/* execve failed */
			perror("Error");
			free(line);
			exit(EXIT_FAILURE);
		}
		else /* Parent process */
			wait(&status);
	}

	free(line);
	return (0);
}

