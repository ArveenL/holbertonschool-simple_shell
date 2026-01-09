#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_ARGS 64

extern char **environ;

/* check if line is only spaces or tabs */
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

/* split line into argv */
void build_argv(char *line, char **argv)
{
	int i = 0;
	char *token = strtok(line, " \t");

	while (token && i < MAX_ARGS - 1)
	{
		argv[i++] = token;
		token = strtok(NULL, " \t");
	}
	argv[i] = NULL;
}

int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	pid_t pid;
	char *argv[MAX_ARGS];
	int interactive = isatty(STDIN_FILENO);

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

		build_argv(line, argv);
		if (!argv[0])
			continue;

		pid = fork();
		if (pid == 0)
		{
			execve(argv[0], argv, environ);
			perror("execve");
			exit(1);
		}
		wait(NULL);
	}

	free(line);
	return (0);
}

