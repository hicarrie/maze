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
 * Return: 0 on success
 */
int main(int argc, char *argv[])
{
	int *maze; /* 2D array defining maze map */
	SDL_Surface *texSrc[TEX_COUNT]; /* array of loaded textures */
	uint8_t *pixel;
	int i, j, k, x, y; /* loop counters */

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

	/* load textures */
	texSrc[0] = IMG_Load("textures/eagle.png");
	texSrc[1] = IMG_Load("textures/redbrick.png");
	texSrc[2] = IMG_Load("textures/purplestone.png");
	texSrc[3] = IMG_Load("textures/greystone.png");
	texSrc[4] = IMG_Load("textures/bluestone.png");
	texSrc[5] = IMG_Load("textures/mossy.png");
	texSrc[6] = IMG_Load("textures/wood.png");
	texSrc[7] = IMG_Load("textures/colorstone.png");

	/* get colors from texture pixels and put in array */
	for (i = 0; i < TEX_COUNT; i++)
	{
		for (j = 0; j < TEX_HEIGHT; j++)
		{
			for (k = 0; k < TEX_WIDTH; k++)
			{
				pixel = (uint8_t *)texSrc[i]->pixels + k * texSrc[i]->pitch + j * texSrc[i]->format->BytesPerPixel;
				tiles[i][j][k] = pixel[0] | pixel[1] << 8 | pixel[2] << 16;;
			}
		}
	}

	/* start SDL and create window and renderer */
	if (!initSDL())
		return (1);

	/* loops until user exits by ESC or closing window */
	while (!quit())
	{
		/* draw ceiling and floor */
		renderBG();

		/* draw walls */
	        renderWalls(maze);

		/* draw buffer */
		SDL_UpdateTexture(texture, NULL, buffer, SCREEN_WIDTH * 4);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, NULL);

		/* update screen */
		/* SDL_RenderPresent(renderer); */

		/* clear buffer */
		for (x = 0; x < SCREEN_WIDTH; x++)
		{
			for (j = 0; j < SCREEN_HEIGHT; j++)
			{
				buffer[j][x] = 0;
			}
		}

		/* handles user input */
		input(maze);
	}

	/* close SDL, renderer, and window */
	closeSDL();

	free(maze);

	return (0);
}
