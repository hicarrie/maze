#include "maze.h"

/**
 * renderBG - draws ceiling and floor
 * Return: void
 */
void renderBG(void)
{
	SDL_Rect ceiling; /* rect for top half of window */
	SDL_Rect floor; /* rect for bottom half of window */
	int i, j;

	ceiling.x = 0;
	ceiling.y = 0;
	ceiling.w = SCREEN_WIDTH;
	ceiling.h = SCREEN_HEIGHT / 2;

	floor.x = 0;
	floor.y = SCREEN_HEIGHT / 2;
	floor.w = SCREEN_WIDTH;
	floor.h = SCREEN_HEIGHT / 2;

	/* draw ceiling */
	for (i = ceiling.x; i < ceiling.x + ceiling.w; i++)
	{
		for (j = ceiling.y; j < ceiling.y + ceiling.h; j++)
			buffer[j][i] = 0;
	}

	/* draw floor */
	for (i = floor.x; i < floor.x + floor.w; i++)
	{
		for (j = floor.y; j < floor.y + floor.h; j++)
			buffer[j][i] = 1;
	}
}
