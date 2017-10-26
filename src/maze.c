#include "maze.h"

/* global variables */
SDL_Window *window;
SDL_Renderer *renderer;
double posX, posY;
double dirX, dirY;
double planeX, planeY;
double time;

/**
 * main - renders maze
 * @argc - number of arguments
 * @argv - pointer to array of arguments
 * Return: 0 on success
 */
int main(int argc, char *argv[])
{
	int **maze; /* 2D array defining maze */

	SDL_Rect ceiling; /* rect for top half of window */

	bool quit; /* main loop flag */

	ceiling.x = 0;
	ceiling.y = 0;
	ceiling.w = SCREEN_WIDTH;
	ceiling.h = SCREEN_HEIGHT / 2;

	posX = 1;
	posY = 12;
	dirX = 1;
	dirY = -0.66;
	planeX = 0;
	planeY = 0.66;
	time = 0;
	quit = false;

	/* parse maze file */
	maze = parseMap(argv[1], maze);

	/* start SDL and create window */
	if (!initSDL())
		return (1);

	while (!quit)
	{
		/* clear screen */
		SDL_SetRenderDrawColor(renderer, 0x1E, 0x29, 0x34, 0xFF);
		SDL_RenderClear(renderer);

		/* draw ceiling */
		SDL_SetRenderDrawColor(renderer, 0x59, 0x85, 0x94, 0xFF);
		SDL_RenderFillRect(renderer, &ceiling);

		/* handle input */
		quit = eventHandler(maze);

		/* cast rays*/
		raycaster(maze);
	}

	/* close renderer and window */
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	freeMap(maze);

	return (0);
}
