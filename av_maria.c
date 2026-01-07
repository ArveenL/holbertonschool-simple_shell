#include <stdio.h>

/**
 * main - prints all command-line arguments, one per line
 * @ac: argument count (unused)
 * @av: array of argument strings
 *
 * Description: Loops through the NULL-terminated array `av` and prints
 * each argument on a new line. Does not use `ac`.
 *
 * Return: 0 on success
 */

int main(int ac, char **av)
{
	int i = 0;

	(void)ac;

	while (av[i])
	{
		printf("%s\n", av[i]);
		i++;
	}

	return (0);
}
