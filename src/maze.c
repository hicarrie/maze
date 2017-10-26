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
 * @argc: number of arguments
 * @argv: pointer to array of arguments
 * Return: 0 on success
 */
int main(int argc, char *argv[])
{
	int **maze; /* 2D array defining maze */

	posX = 1;
	posY = 12;
	dirX = 1;
	dirY = -0.66;
	planeX = 0;
	planeY = 0.66;
	time = 0;

	/* parse maze file */
	maze = parseMap(argv[1], maze);
	if (maze == NULL)
		return (1);

	/* start SDL and create window */
	if (!initSDL())
		return (1);

	/* loops until user exits by ESC or closing window */
	while (!quit())
	{
		/* draw ceiling and floor */
		renderBG();

		/* handles user input */
		inputHandler(maze);

		/* cast rays*/
		raycaster(maze);
	}

	/* close renderer and window */
	closeSDL();

	freeMap(maze);

	return (0);
}
