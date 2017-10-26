#include "maze.h"

/**
 * renderWalls - casts rays and renders walls
 * @maze: 2D array defining maze map
 * Return: void
 */
void renderWalls(int **maze)
{
	double cameraX; /* X coordinate in camera space */
	point_t rayPos; /* X/Y coordinates of ray position */
	point_t rayDir; /* direction of X/Y coordinates of ray position */
	point_t posToNext; /* length of ray from current position to next X/Y sides */
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
			if (maze[map.x][map.y] > 0)
				hit = 1;
		}

		/* calculate distance projected in camera direction */
		if (side == 0)
			distToWall = (map.x - rayPos.x + (1 - step.x) / 2) / rayDir.x;
		else
			distToWall = (map.y - rayPos.y + (1 - step.y) / 2) / rayDir.y;

		/* draw wall slice */
		drawSlice(distToWall, x, side);
	}

	/* update screen */
	SDL_RenderPresent(renderer);
}


/**
 * drawSlice - draws slice/column of wall
 * @distToWall: distance to wall from camera
 * @x: number of ray casted
 * Return: void
 */
void drawSlice(double distToWall, int x, int side)
{
	int sliceHeight; /* height of wall slice to draw */
	int drawStart; /* lowest pixel of wall slice */
	int drawEnd; /* highest pixel of wall slice */

	/* calculate height of wall slice to draw on screen */
	sliceHeight = (int)(SCREEN_HEIGHT / distToWall);

	/* calculate lowest and highest pixel of wall slice */
	drawStart = -sliceHeight / 2 + SCREEN_HEIGHT / 2;
	if (drawStart < 0)
		drawStart = 0;

	drawEnd = sliceHeight / 2 + SCREEN_HEIGHT / 2;
	if (drawEnd >= SCREEN_HEIGHT)
		drawEnd = SCREEN_HEIGHT - 1;

	/* set wall colors depending if it's N/S or W/E */
	if (side == 0)
		SDL_SetRenderDrawColor(renderer, 0xF7, 0xF7, 0xF7, 0xFF);
	else if (side == 1)
		SDL_SetRenderDrawColor(renderer, 0xDE, 0xDE, 0xDE, 0xFF);

	/* draw pixels of wall slice as a vertical line */
	SDL_RenderDrawLine(renderer, x, drawStart, x, drawEnd);
}
