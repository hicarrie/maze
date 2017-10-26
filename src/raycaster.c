#include "maze.h"

/**
 * raycaster -
 *
 */
void raycaster(int **maze)
{
	double cameraX; /* X coordinate in camera space */
	double rayPosX; /* X coordinate of ray position */
	double rayPosY; /* Y coordinate of ray position */
	double rayDirX; /* direction of X coordinate of ray position */
	double rayDirY; /* direction of Y coordinate of ray position */

	double posToNextX; /* length of ray from current position to next X side */
	double posToNextY; /* length of ray from current position to next Y side */
	double distToNextX; /* length of ray from X side to next X side */
	double distToNextY; /* length of ray from Y side to next Y side */
	double distToWall; /* distance from camera to wall  */

	SDL_Point map; /* X/Y coordinates of box of maze currently in */
	SDL_Point step; /* X/Y direction to step in - always 1 or -1 */

	int hit; /* check if wall was hit */
	int side; /* check if wall hit was N/S or E/W */

	int sliceHeight; /* height of wall slice to draw */
	int drawStart; /* lowest pixel of wall slice */
	int drawEnd; /* highest pixel of wall slice */

	int x; /* column counter for raycasting */

	/* cast ray x for every column w */
	for (x = 0; x < SCREEN_WIDTH; x++)
	{
		/* calculate ray position and direction */
		cameraX = 2 * x / (double)(SCREEN_WIDTH) - 1;
		rayPosX = posX;
		rayPosY = posY;
		rayDirX = dirX + planeX * cameraX;
		rayDirY = dirY + planeY * cameraX;

		/* grid position on maze */
		map.x = (int)(rayPosX);
		map.y = (int)(rayPosY);

		/* measure distance to next X or Y intersection */
		distToNextX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
		distToNextY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY));

		/* calculate step and initial distance from position to X or Y */
		if (rayDirX < 0)
		{
			step.x = -1;
			posToNextX = (rayPosX - map.x) * distToNextX;
		}
		else
		{
			step.x = 1;
			posToNextX = (map.x + 1.0 - rayPosX) * distToNextX;
		}

		if (rayDirY < 0)
		{
			step.y = -1;
			posToNextY = (rayPosY - map.y) * distToNextY;
		}
		else
		{
			step.y = 1;
			posToNextY = (map.y + 1.0 - rayPosY) * distToNextY;
		}

		hit = 0;
		while (hit == 0)
		{
			/* move to next maze square in X or Y direction */
			if (posToNextX < posToNextY)
			{
				posToNextX += distToNextX;
				map.x += step.x;
				side = 0;
			}
			else
			{
				posToNextY += distToNextY;
				map.y += step.y;
				side = 1;
			}

			/* check if ray hit a wall */
			if (maze[map.x][map.y] > 0)
				hit = 1;
		}

		/* calculate distance projected in camera direction */
		if (side == 0)
			distToWall = (map.x - rayPosX + (1 - step.x) / 2) / rayDirX;
		else
			distToWall = (map.y - rayPosY + (1 - step.y) / 2) / rayDirY;

		/* calculate height of wall slice to draw on screen */
		sliceHeight = (int)(SCREEN_HEIGHT / distToWall);

		/* calculate lowest and highest pixel of wall slice */
		drawStart = -sliceHeight / 2 + SCREEN_HEIGHT / 2;
		if (drawStart < 0)
			drawStart = 0;
		drawEnd = sliceHeight / 2 + SCREEN_HEIGHT / 2;
		if (drawEnd >= SCREEN_HEIGHT)
			drawEnd = SCREEN_HEIGHT - 1;

		if (side == 0)
			SDL_SetRenderDrawColor(renderer, 230, 230, 230, SDL_ALPHA_OPAQUE);
		else if (side == 1)
			SDL_SetRenderDrawColor(renderer, 200, 200, 200, SDL_ALPHA_OPAQUE);

		/* draw pixels of wall slice as a vertical line */
		SDL_RenderDrawLine(renderer, x, drawStart, x, drawEnd);
	}

	/* update screen */
	SDL_RenderPresent(renderer);
}
