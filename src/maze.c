#include "maze.h"

/* global variables */
SDL_Window *window;
SDL_Renderer *renderer;
point_t pos;
point_t dir;
point_t plane;
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

	/* initial values for global variables */
	pos.x = 1;
	pos.y = 12;
	dir.x = 1;
	dir.y = -0.66;
	plane.x = 0;
	plane.y = 0.66;
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
