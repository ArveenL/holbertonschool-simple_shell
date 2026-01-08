#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

extern char **environ;

/**
 * main - simple shell 0.1 for Holberton
 *
 * Description: Reads a single-word command from stdin, trims whitespace,
 * forks a child, executes the command using execve, handles errors, and loops
 * until EOF. Prints the prompt only when input is from a terminal.
 *
 * Return: 0 on success, or exits with 1 if execve fails
 */
int main(void)
{
    char *line;
    size_t len;
    pid_t pid;

    line = NULL;
    len = 0;

    while (1)
    {
        if (isatty(STDIN_FILENO))
            printf("#cisfun$ "); /* prompt only if terminal */

        if (getline(&line, &len, stdin) == -1)
            break;

        /* trim leading whitespace */
        while (*line == ' ' || *line == '\t' || *line == '\n')
            line++;

        /* remove trailing newline and spaces */
        line[strcspn(line, "\n")] = 0;
        if (line[0] == '\0') /* empty input */
            continue;

        pid = fork();
        if (pid == 0) /* child */
        {
            char *args[2];
            args[0] = line;
            args[1] = NULL;
            execve(args[0], args, environ); /* run command */
            perror("./shell"); /* execve failed */
            exit(1);
        }
        else if (pid > 0) /* parent */
        {
            wait(NULL); /* wait for child */
        }
        else /* fork failed */
        {
            perror("fork");
        }
    }

    free(line);
    return (0);
}

