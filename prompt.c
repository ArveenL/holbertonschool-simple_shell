#include <stdio.h>
#include <stdlib.h>

/**
 * main - prints "$ ", reads a command from the user,
 *        and prints it back. Repeats until EOF (Ctrl+D).
 *
 * Return: 0 on success
 */
int main(void)
{
    char *line = NULL;  /* pointer to store input line */
    size_t len = 0;     /* buffer size, getline will update */
    ssize_t nread;      /* number of characters(bytes) read */

    while (1)
    {
        printf("$ ");  /* print shell-style prompt */

        nread = getline(&line, &len, stdin); /* read line from user */
        if (nread == -1)  /* EOF or error */
        {
            printf("\n"); /* print newline for clean exit */
            break;        /* exit the loop */
        }

        printf("%s", line); /* echo the command back */
    }

    free(line); /* free memory allocated by getline */
    return (0);
}
