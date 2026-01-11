#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

extern char **environ;

/* Function prototypes */
void display_prompt(void);
char *read_command(void);
void execute_command(char *command, char *argv[], int count);
void print_error(char *name, int count, char *command);

/**
 * main - Entry point for simple shell
 * @argc: Argument count
 * @argv: Argument vector
 *
 * Return: 0 on success
 */
int main(int argc, char *argv[])
{
	char *command = NULL;
	int interactive = isatty(STDIN_FILENO);
	int command_count = 0;
	(void)argc;

	while (1)
	{
		if (interactive)
			display_prompt();

		command = read_command();

		if (command == NULL) /* EOF (Ctrl+D) */
		{
			if (interactive)
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		if (command[0] == '\0') /* Empty line */
		{
			free(command);
			continue;
		}

		command_count++;
		execute_command(command, argv, command_count);
		free(command);
	}

	return (0);
}

/**
 * display_prompt - Display shell prompt
 */
void display_prompt(void)
{
	write(STDOUT_FILENO, "#cisfun$ ", 9);
	fflush(stdout);
}

/**
 * read_command - Read command from stdin
 *
 * Return: Command string or NULL on EOF
 */
char *read_command(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read_len;

	read_len = getline(&line, &len, stdin);

	if (read_len == -1)
	{
		free(line);
		return (NULL);
	}

	/* Remove newline */
	if (line[read_len - 1] == '\n')
		line[read_len - 1] = '\0';

	return (line);
}

/**
 * execute_command - Execute a command
 * @command: Command to execute
 * @argv: Program arguments (for error messages)
 * @count: Command counter for error messages
 */
void execute_command(char *command, char *argv[], int count)
{
	pid_t pid;
	int status;
	char *args[2];
	struct stat st;

	/* Check if file exists and is executable */
	if (stat(command, &st) == -1)
	{
		print_error(argv[0], count, command);
		return;
	}

	pid = fork();

	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}

	if (pid == 0) /* Child process */
	{
		args[0] = command;
		args[1] = NULL;

		if (execve(command, args, environ) == -1)
		{
			perror(argv[0]);
			exit(EXIT_FAILURE);
		}
	}
	else /* Parent process */
	{
		wait(&status);
	}
}

/**
 * print_error - Print error message
 * @name: Program name
 * @count: Command count
 * @command: Command that failed
 */
void print_error(char *name, int count, char *command)
{
	char *err_msg = ": No such file or directory\n";
	char buffer[20];
	int len;

	write(STDERR_FILENO, name, strlen(name));
	write(STDERR_FILENO, ": ", 2);

	/* Print command count */
	len = sprintf(buffer, "%d", count);
	write(STDERR_FILENO, buffer, len);

	write(STDERR_FILENO, ": ", 2);
	write(STDERR_FILENO, command, strlen(command));
	write(STDERR_FILENO, err_msg, strlen(err_msg));
}
