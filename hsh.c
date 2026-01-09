#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

extern char **environ;

/**
 * main - simple shell
 * Return: Always 0
 */
int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	pid_t pid;
	char *args[2];

	while (1)
	{
		write(STDOUT_FILENO, "#cisfun$ ", 9);

		read = getline(&line, &len, stdin);
		if (read == -1)
			break;

		if (line[read - 1] == '\n')
			line[read - 1] = '\0';

		if (*line == '\0')
			continue;

		pid = fork();
		if (pid == 0)
		{
			args[0] = line;
			args[1] = NULL;
			execve(args[0], args, environ);
			perror("execve");
			exit(EXIT_FAILURE);
		}
		wait(NULL);
	}

	free(line);
	return (0);
}

