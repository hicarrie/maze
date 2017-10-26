#include "maze.h"

/**
 * renderBG - draws ceiling and floor
 * Return: void
 */
void renderBG(void)
{
	SDL_Rect ceiling; /* rect for top half of window */
	SDL_Rect floor; /* rect for bottom half of window */

	ceiling.x = 0;
	ceiling.y = 0;
	ceiling.w = SCREEN_WIDTH;
	ceiling.h = SCREEN_HEIGHT / 2;

	floor.x = 0;
	floor.y = SCREEN_HEIGHT / 2;
	floor.w = SCREEN_WIDTH;
	floor.h = SCREEN_HEIGHT / 2;

	/* draw ceiling */
	SDL_SetRenderDrawColor(renderer, 0x59, 0x85, 0x94, 0xFF);
	SDL_RenderFillRect(renderer, &ceiling);

	/* draw floor */
	SDL_SetRenderDrawColor(renderer, 0x1E, 0x29, 0x34, 0xFF);
	SDL_RenderFillRect(renderer, &floor);
}
