#include "maze.h"

/* global variables */
SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *texture;
uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH];
uint32_t tiles[TEX_COUNT][TEX_HEIGHT][TEX_WIDTH];
point_t pos;
point_t dir;
point_t plane;
double time;

/**
 * main - renders maze
 * @argc: number of arguments
 * @argv: pointer to array of arguments
 * Return: 0 on success, 1 on failure
 */
int main(int argc, char *argv[])
{
	int *maze; /* 2D array defining maze map */

	/* initial values for global variables */
	pos.x = 1;
	pos.y = 12;
	dir.x = 1;
	dir.y = -0.66;
	plane.x = 0;
	plane.y = 0.66;
	time = 0;

	/* start SDL and create window and renderer */
	if (!initSDL())
		return (1);

	/* parse maze file */
	maze = parseMap(argv[1], maze);
	if (maze == NULL)
		return (1);

	loadTextures(argv[1]);

	/* loops until user exits by ESC or closing window */
	while (!quit())
	{
		/* draw walls, floor, and ceiling */
		raycaster(maze);

		/* handles user input */
		input(maze);
	}

	/* close SDL, renderer, and window */
	closeSDL();

	free(maze);

	return (0);
}
