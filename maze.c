#include "maze.h"

/* global SDL variables */
SDL_Window *window;
SDL_Renderer *renderer;
int mapX; /* X of current grid position on map */
int mapY; /* Y of current grid position on map */



/**
 * main - renders maze
 * @argc - number of arguments
 * @argv - pointer to array of arguments
 * Return: 0 on success
 */
int main(int argc, char *argv[])
{
	SDL_Rect ceiling; /* rect for top half of window */
	int **map; /* 2D array defining map */

	SDL_Event event;
	bool quit; /* main loop flag */

	double ray;
	double fov; /* field of view */
	double rayAngle;
	double incrementAngle; /* increments ray angle for each column */
	double cameraAngle; /* direction of camera */
	double cameraX; /* x position of camera */
	double cameraY; /* y position of camera */
	double theta; /* for calculating angle difference between camera and ray directions */

	double verticalDist; /* distance to wall by checking X intersections */
	double horizontalDist; /* distance to wall by checking Y intersections */
	double perpendicularDist; /* perpendicular distance from camera to wall */

	double sliceHeight; /* height of wall slice to draw */

	int x; /* column counter for raycasting */

	ceiling.x = 0;
	ceiling.y = 0;
	ceiling.w = SCREEN_WIDTH;
	ceiling.h = SCREEN_HEIGHT / 2;

	quit = false;

	fov = 60;
	incrementAngle = (double)fov / (double)SCREEN_WIDTH;
	cameraAngle = 0.66;
	mapX = 1;
	mapY = 12;

	verticalDist = INT_MAX;
	horizontalDist = INT_MAX;
	perpendicularDist = 0;

	/* parse map file */
	map = parseMap(argv[1], map);

	/* start SDL and create window */
	initSDL();

	while (!quit)
	{
		/* clear screen */
		SDL_SetRenderDrawColor(renderer, 0x1E, 0x29, 0x34, 0xFF);
		SDL_RenderClear(renderer);

		/* draw ceiling */
		SDL_SetRenderDrawColor(renderer, 0x59, 0x85, 0x94, 0xFF);
		SDL_RenderFillRect(renderer, &ceiling);

		cameraX = mapX;
		cameraY = mapY;

		/* cast ray for every column */
		for (ray = (double)(fov/2); ray > (double)(-fov/2); ray-= incrementAngle)
		{

			rayAngle = degreesToRadians(cameraAngle + ray);

			verticalDist = getVertDist(rayAngle, cameraX, cameraY, map, mapX, mapY);
			/* horizontalDist = getHorizDist(rayAngle, cameraX); */

			printf("%d\n", verticalDist);
			/* printf("%d\n", horizontalDist); */

			theta = abs(rayAngle - degreesToRadians(cameraAngle));
		}

	}

	/* close renderer and window */
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	freeMap(map);

	return (0);
}
