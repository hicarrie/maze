#include "maze.h"

/**
 * freeMap - frees 2D array containing map information
 * @map: 2D array containing map information
 * Return: void
 */
void freeMap(int **map)
{
	int i;

	for (i = 0; i < MAP_HEIGHT; i++)
		free(map[i]);

	free(map);
}
