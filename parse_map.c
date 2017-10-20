#include "maze.h"

/**
 * parseMap - parses 2D array from file to create maze
 * @filename: name of file containing map information
 * @map_buffer: buffer to read map information into
 * Return: pointer to buffer containing read map information
 */
int **parseMap(const char *filename, int **map)
{
	FILE *fp;
	char row[MAP_WIDTH * 2]; /* multiply by 2 to account for spaces */
	char *number;
	int i, j;

	if (filename == NULL)
		return (NULL);

	fp = fopen(filename, "r");
	if (fp == NULL)
		return (NULL);

	map = malloc(sizeof(int *) * MAP_HEIGHT);
	if (map == NULL)
		return (NULL);

	i = 0;
        while (fgets(row, sizeof(row), fp) != NULL)
	{
		if (strlen(row) <= 1)
			continue;

		map[i] = malloc(sizeof(int) * MAP_WIDTH);
		if (map[i] == NULL)
			return (NULL);

		number = strtok(row, "\n ");
		j = 0;
		while (number != NULL)
		{
			map[i][j] = atoi(number);
			number = strtok(NULL, "\n ");
			j++;
		}
		i++;
	}

	fclose(fp);

	return (map);
}
