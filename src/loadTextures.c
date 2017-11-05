#include "maze.h"

/**
 * loadTextures - loads textures from file and puts pixel information in buffer
 * Return: void
 */
void loadTextures(void)
{
	SDL_Surface *texSrc[TEX_COUNT]; /* array of loaded textures */
	uint8_t *pixel; /* color value of pixel at given coordinate */
	int i, j, k; /* loop counters */

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
				pixel = (uint8_t *)texSrc[i]->pixels
					+ k * texSrc[i]->pitch + j *
					texSrc[i]->format->BytesPerPixel;

				tiles[i][j][k] = pixel[0] | pixel[1] << 8
					| pixel[2] << 16;;
			}
		}
	}

	/* free SDL_Surfaces */
	for (i = 0; i < TEX_COUNT; i++)
	{
		SDL_FreeSurface(texSrc[i]);
		texSrc[i] = NULL;
	}
}
