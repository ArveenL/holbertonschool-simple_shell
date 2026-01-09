#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

extern char **environ;

/* checks if line contains only spaces or tabs */
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

int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	pid_t pid;
	char *argv[2];
	int interactive = isatty(STDIN_FILENO);

	while (1)
	{
		if (interactive)
			write(STDOUT_FILENO, "#cisfun$ ", 9);

		read = getline(&line, &len, stdin);
		if (read == -1)   /* EOF ONLY */
			break;

		if (line[read - 1] == '\n')
			line[read - 1] = '\0';

		if (*line == '\0' || is_empty(line))
			continue;

		pid = fork();
		if (pid == 0)
		{
			argv[0] = line;
			argv[1] = NULL;
			execve(argv[0], argv, environ);
			exit(1);
		}
		wait(NULL);
	}

	free(line);
	return (0);
}

