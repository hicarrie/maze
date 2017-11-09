#include "maze.h"

/**
 * parseMap - parses 2D array from file to create maze
 * @filename: name of file containing map information
 * @map: 2D array of map information
 * Return: pointer to 2D array containing map information
 */
int *parseMap(char *filename, int *map)
{
	FILE *fp;
	char row[MAP_WIDTH * 2]; /* multiply by 2 to account for spaces in map file */
	char *number;
	int i, j;

	fp = fopen(filename, "r");
	if (fp == NULL)
	{
		perror("Map file could not be opened");
		return (NULL);
	}

	map = malloc(sizeof(int) * MAP_HEIGHT * MAP_WIDTH);
	if (map == NULL)
		return (NULL);

	i = 0;
	while (fgets(row, sizeof(row), fp) != NULL)
	{
		/* if row contains only newline character, go to next row */
		if (strlen(row) <= 1)
			continue;

		number = strtok(row, "\n ");

		j = 0;
		while (number != NULL)
		{
			map[i * MAP_WIDTH + j] = atoi(number);
			number = strtok(NULL, "\n ");
			j++;
		}
		i++;
	}
	fclose(fp);
	return (map);
}
