#include <stdio.h>
#include <string.h>

/**
 * main - splits a string into words separated by spaces
 *
 * Return: 0 on success
 */
int main(void)
{
    char line[] = "hello world test";
    char *token;

    token = strtok(line, " ");

    while (token != NULL)
    {
        printf("%s\n", token);
        token = strtok(NULL, " ");
    }

    return (0);
}

