#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_LINE 1024

/**
 * main - simple shell
 *
 * Return: 0
 */
int main(void)
{
    char line[MAX_LINE];
    char *args[100];
    int interactive = isatty(STDIN_FILENO); /* check if terminal */

    while (1)
    {
        if (interactive)
            printf(":) "); /* prompt only in interactive mode */
        fflush(stdout);

        if (fgets(line, sizeof(line), stdin) == NULL)
            break; /* EOF or Ctrl+D */

        /* remove trailing newline */
        line[strcspn(line, "\n")] = '\0';

        /* split line into arguments */
        int i = 0;
        char *token = strtok(line, " \t\n");
        while (token != NULL)
        {
            args[i++] = token;
            token = strtok(NULL, " \t\n");
        }
        args[i] = NULL;

        if (i == 0)
            continue; /* empty line */

        /* built-in exit */
        if (strcmp(args[0], "exit") == 0)
            break;

        pid_t pid = fork();
        if (pid == -1)
        {
            perror("fork");
            continue;
        }
        if (pid == 0)
        {
            /* child process */
            if (execvp(args[0], args) == -1)
            {
                perror("hsh");
                exit(1);
            }
        }
        else
        {
            /* parent waits */
            int status;
            waitpid(pid, &status, 0);
        }
    }

    if (interactive)
        printf("\n"); /* newline after Ctrl+D in interactive mode */

    return 0;
}

