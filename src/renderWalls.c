#include "maze.h"

/**
 * renderWalls - casts rays and renders walls
 * @maze: 2D array defining maze map
 * Return: void
 */
void renderWalls(int *maze)
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

	int x, j; /* ray counter */

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

		/* draw wall slice */
		drawSlice(maze, map, rayPos, rayDir, distToWall, x, side);
	}

	/* draw buffer */
	SDL_UpdateTexture(texture, NULL, buffer, SCREEN_WIDTH * 4);
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, NULL, NULL);

	/* update screen */
	SDL_RenderPresent(renderer);

	/* clear buffer */
	for (x = 0; x < SCREEN_WIDTH; x++)
	{
		for (j = 0; j < SCREEN_HEIGHT; j++)
		{
			buffer[j][x] = 0;
		}
	}
}


/**
 * drawSlice - draws slice/column of wall
 * @distToWall: distance to wall from camera
 * @x: number of ray casted
 * Return: void
 */
void drawSlice(int *maze, SDL_Point map, point_t rayPos, point_t rayDir, double distToWall, int x, int side)
{
	int sliceHeight; /* height of wall slice to draw */
	int drawStart; /* lowest pixel of wall slice */
	int drawEnd; /* highest pixel of wall slice */
	int tileIndex; /* tile number of current map location */
	double wallX; /* where wall was hit */
	SDL_Point tex; /* X/Y coordinate of pixel of texture */
	uint32_t color; /* color of pixel */
	int y;

	/* calculate height of wall slice to draw on screen */
	sliceHeight = (int)(SCREEN_HEIGHT / distToWall);

	/* calculate lowest and highest pixel of wall slice */
	drawStart = -sliceHeight / 2 + SCREEN_HEIGHT / 2;
	if (drawStart < 0)
		drawStart = 0;

	drawEnd = sliceHeight / 2 + SCREEN_HEIGHT / 2;
	if (drawEnd >= SCREEN_HEIGHT)
		drawEnd = SCREEN_HEIGHT - 1;

	tileIndex = *((int *)maze + map.x * MAP_WIDTH + map.y) - 1;

	if (side == 0)
		wallX = rayPos.y + distToWall * rayDir.y;
	else if (side == 1)
		wallX = rayPos.x + distToWall * rayDir.x;

	/* floor returns the largest integer value less than or equal to wallX */
	wallX -= floor(wallX);

	/* get X coordinate of texture corresponding to ray hit */
	tex.x = (int)(wallX * (double)TEX_WIDTH);
	if (side == 0 && rayDir.x > 0)
		tex.x = TEX_WIDTH - tex.x - 1;
	if (side == 1 && rayDir.y < 0)
		tex.x = TEX_WIDTH - tex.x - 1;

	for (y = drawStart; y < drawEnd; y++)
	{
		if (x > SCREEN_WIDTH | y > SCREEN_HEIGHT)
			return;
		tex.y = ((((y << 1) - SCREEN_HEIGHT + sliceHeight) << (int)log2(TEX_HEIGHT)) / sliceHeight) >> 1;
		color = tiles[tileIndex][tex.x][tex.y];
		/* change brightness of color if the wall depending on wall side */
		if (side == 1)
			color = (color >> 1) & 8355311;

		buffer[y][x] = color;
	}
}
