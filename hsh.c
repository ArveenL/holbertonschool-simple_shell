#include <stdio.h>      /* printf, perror */
#include <stdlib.h>     /* exit, malloc, free */
#include <unistd.h>     /* fork, execve, access */
#include <string.h>     /* strlen */
#include <sys/wait.h>   /* wait */

extern char **environ;

/**
 * main - Simple shell 0.1
 * Description: A minimal UNIX command line interpreter.
 * Return: 0 on success
 */
int main(void)
{
    char *line;
    size_t len;
    ssize_t nread;
    pid_t child_pid;
    int status;
    char *argv[2];

    line = NULL;
    len = 0;

    while (1)
    {
        printf("$ ");  /* display prompt */

        nread = getline(&line, &len, stdin);  /* read user input */

        if (nread == -1)  /* handle EOF (Ctrl+D) */
        {
            printf("\n");
            break;
        }

        if (line[nread - 1] == '\n')  /* remove trailing newline */
            line[nread - 1] = '\0';

        if (line[0] == '\0')  /* empty command, just reprompt */
            continue;

        /* set up argv for execve */
        argv[0] = line;
        argv[1] = NULL;

        child_pid = fork();
        if (child_pid == -1)
        {
            perror("fork");
            continue;
        }

        if (child_pid == 0)  /* child process */
        {
            if (access(line, X_OK) == 0)  /* check executable */
                execve(line, argv, environ);
            perror("Error");  /* if execve fails */
            exit(EXIT_FAILURE);
        }
        else  /* parent process */
        {
            wait(&status);  /* wait for child */
        }
    }

    free(line);
    return 0;
}

