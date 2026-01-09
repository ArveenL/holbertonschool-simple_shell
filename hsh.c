#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

extern char **environ;

/**
 * is_empty - checks if a string is only spaces or tabs
 * @s: string
 * Return: 1 if empty, 0 otherwise
 */
int is_empty(char *s)
{
	while (*s)
	{
		if (*s != ' ' && *s != '\t')
			return (0);
		s++;
	}
	return (1);
}

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
	int interactive;

	interactive = isatty(STDIN_FILENO);

	while (1)
	{
		if (interactive)
			write(STDOUT_FILENO, "#cisfun$ ", 9);

		read = getline(&line, &len, stdin);
		if (read == -1)
			break;

		if (line[read - 1] == '\n')
			line[read - 1] = '\0';

		if (*line == '\0' || is_empty(line))
			continue;

		pid = fork();
		if (pid == 0)
		{
			args[0] = line;
			args[1] = NULL;
			execve(args[0], args, environ);
			exit(EXIT_FAILURE);
		}
		wait(NULL);
	}

	free(line);
	return (0);
}

