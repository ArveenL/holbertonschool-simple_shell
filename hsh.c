#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    char *line;
    size_t len;
    char *args[64];
    char *token;
    int i;
    pid_t pid;

    line = NULL;
    len = 0;

    while (1)
    {
        printf("$ "); /* prompt */
        if (getline(&line, &len, stdin) == -1)
            break;
        line[strcspn(line, "\n")] = 0; /* remove newline */

        i = 0;
        token = strtok(line, " ");
        while (token != NULL)
        {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL; /* end with NULL */

        if (args[0] == NULL)
            continue;

        pid = fork();
        if (pid == 0)
        {
            execvp(args[0], args);
            perror("exec");
            exit(1);
        }
        else if (pid > 0)
        {
            waitpid(pid, NULL, 0);
        }
        else
        {
            perror("fork");
        }
    }

    free(line);
    return 0;
}

