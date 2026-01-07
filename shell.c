#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/**
 * main - main
 * @ac: Argument count (unused)
 * @av: Argument vector (unused)
 * @env: Environment variables
 *
 * Description:
 * Displays a prompt, waits for user input, executes commands
 * using their full path. Handles EOF (Ctrl+D).
 *
 * Return: Always 0
 */
int main(int ac, char **av, char **env)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;
	pid_t pid;

	(void)ac;
	(void)av;

	while (1)
	{
		/* Display prompt */
		write(STDOUT_FILENO, "bigoolooa1 ", 9);

		/* Read user input */
		nread = getline(&line, &len, stdin);
		if (nread == -1)
		{
			/* Handle EOF (Ctrl+D) */
			write(STDOUT_FILENO, "\n", 1);
			break;
		}

		/* Remove trailing newline */
		line[nread - 1] = '\0';

		/* Ignore empty input */
		if (line[0] == '\0')
			continue;

		/* Create child process */
		pid = fork();
		if (pid == 0)
		{
			char *argv[] = {line, NULL};

			/* Execute command */
			execve(line, argv, env);

			/* execve only returns on failure */
			perror("./shell");
			exit(1);
		}
		else
		{
			/* Parent waits for child */
			wait(NULL);
		}
	}

	free(line);
	return (0);
}
