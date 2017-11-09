#include "maze.h"

/**
 * raycaster - casts rays and renders walls, floor, and ceiling
 * @maze: 2D array defining maze map
 * @textured: True if user enabled textures, otherwise False
 * Return: void
 */
void raycaster(int *maze, bool textured)
{
	double cameraX; /* X coordinate in camera space */
	point_t rayPos; /* X/Y coordinates of ray pos */
	point_t rayDir; /* direction of X/Y coordinates of ray pos */
	point_t posToNext; /* length of ray from current pos to next X/Y side */
	point_t distToNext; /* length of ray from X/Y side to next X/Y side */
	double distToWall; /* distance from camera to wall  */

	SDL_Point map; /* X/Y coordinates of box of maze currently in */
	SDL_Point step; /* X/Y direction to step in - always 1 or -1 */

	int hit; /* check if wall was hit */
	int side; /* check if wall hit was N/S or E/W */

	int x; /* ray counter */

	/* cast ray x for every column w */
	for (x = 0; x < SCREEN_WIDTH; x++)
	{
		/* calculate ray position and direction */
		cameraX = 2 * x / (double)(SCREEN_WIDTH) - 1;
		rayPos.x = pos.x;
		rayPos.y = pos.y;
		rayDir.x = dir.x + plane.x * cameraX;
		rayDir.y = dir.y + plane.y * cameraX;

		/* grid position on maze */
		map.x = (int)(rayPos.x);
		map.y = (int)(rayPos.y);

		/* measure distance to next X or Y intersection */
		distToNext.x = sqrt(1 + (pow(rayDir.y, 2)) / pow(rayDir.x, 2));
		distToNext.y = sqrt(1 + (pow(rayDir.x, 2)) / pow(rayDir.y, 2));

		/* calculate step and distance from pos to first X or Y */
		if (rayDir.x < 0)
		{
			step.x = -1;
			posToNext.x = (rayPos.x - map.x) * distToNext.x;
		}
		else
		{
			step.x = 1;
			posToNext.x = (map.x + 1.0 - rayPos.x) * distToNext.x;
		}

		if (rayDir.y < 0)
		{
			step.y = -1;
			posToNext.y = (rayPos.y - map.y) * distToNext.y;
		}
		else
		{
			step.y = 1;
			posToNext.y = (map.y + 1.0 - rayPos.y) * distToNext.y;
		}

		/* move to next maze square in X or Y dir until wall is hit */
		hit = 0;
		while (hit == 0)
		{
			if (posToNext.x < posToNext.y)
			{
				posToNext.x += distToNext.x;
				map.x += step.x;
				side = 0;
			}
			else
			{
				posToNext.y += distToNext.y;
				map.y += step.y;
				side = 1;
			}

			/* check if ray hit a wall */
			if (*((int *)maze + map.x * MAP_WIDTH + map.y) > 0)
				hit = 1;
		}

		/* calculate distance projected in camera direction */
		if (side == 0)
			distToWall = (map.x - rayPos.x + (1 - step.x) / 2) / rayDir.x;
		else
			distToWall = (map.y - rayPos.y + (1 - step.y) / 2) / rayDir.y;

		/* draw walls to buffer */
		renderWalls(maze, map, rayPos, rayDir, distToWall, x, side, textured);
	}

	/* draw updated buffer with walls, floor, and ceiling to renderer */
	updateRenderer(textured);
}
