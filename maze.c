#include "maze.h"

/* global SDL variables */
SDL_Window *window;
SDL_Renderer *renderer;

/**
 * main - renders maze
 * @argc - number of arguments
 * @argv - pointer to array of arguments
 * Return: 0 on success
 */
int main(int argc, char *argv[])
{
	int **maze; /* 2D array defining maze */

	double posX, posY; /* X and Y start position */
	double dirX, dirY; /* initial direction vector */
	double planeX, planeY; /* camera plane */
	double oldDirX, oldPlaneX; /* previous X direction vector and X plane */

	double cameraX; /* X coordinate in camera space */
	double rayPosX; /* X coordinate of ray position */
	double rayPosY; /* Y coordinate of ray position */
	double rayDirX; /* direction of X coordinate of ray position */
	double rayDirY; /* direction of Y coordinate of ray position */

	SDL_Point player; /* X/Y coordinates of box of maze player is in */

	double posToNextX; /* length of ray from current position to next X side */
	double posToNextY; /* length of ray from current position to next Y side */
	double distToNextX; /* length of ray from X side to next X side */
	double distToNextY; /* length of ray from Y side to next Y side */
	double distToWall; /* distance from camera to wall  */

	int stepX; /* X direction to step in - always 1 or -1 */
	int stepY; /* Y direction to step in - always 1 or -1 */

	int hit; /* check if wall was hit */
	int side; /* check if wall hit was N/S or E/W */

	int sliceHeight; /* height of wall slice to draw */
	int drawStart; /* lowest pixel of wall slice */
	int drawEnd; /* highest pixel of wall slice */

	double moveSpeed; /* constant in squares/second */
	double rotateSpeed; /* constant in radians/second */

	double time; /* time of current frame */
	double oldTime; /*time of previous frame */
	double frameTime; /* time the frame has taken in seconds */

	const uint8_t *keystate; /* current key state */

	SDL_Event event; /* event listener */
	SDL_Rect ceiling; /* rect for top half of window */

	bool quit; /* main loop flag */

	int x; /* column counter for raycasting */

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
	oldTime = 0;
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

		/* cast ray x for every column w */
		for (x = 0; x < SCREEN_WIDTH; x++)
		{
			/* check if user exits */
			while (SDL_PollEvent(&event) != 0)
			{
				/* user requests quit */
				if (event.type == SDL_QUIT)
					quit = true;

				if (event.type == SDL_KEYDOWN &&
				    event.key.keysym.sym == SDLK_ESCAPE)
					quit = true;
			}

			/* calculate ray position and direction */
			cameraX = 2 * x / (double)(SCREEN_WIDTH) - 1;
			rayPosX = posX;
			rayPosY = posY;
			rayDirX = dirX + planeX * cameraX;
			rayDirY = dirY + planeY * cameraX;

			/* grid position on maze */
			player.x = (int)(rayPosX);
			player.y = (int)(rayPosY);

			/* measure distance to next X or Y intersection */
			distToNextX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
			distToNextY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY));

			/* calculate step and initial distance from position to X or Y */
			if (rayDirX < 0)
			{
				stepX = -1;
				posToNextX = (rayPosX - player.x) * distToNextX;
			}
			else
			{
				stepX = 1;
				posToNextX = (player.x + 1.0 - rayPosX) * distToNextX;
			}

			if (rayDirY < 0)
			{
				stepY = -1;
				posToNextY = (rayPosY - player.y) * distToNextY;
			}
			else
			{
				stepY = 1;
				posToNextY = (player.y + 1.0 - rayPosY) * distToNextY;
			}

			hit = 0;
			while (hit == 0)
			{
				/* move to next maze square in X or Y direction */
				if (posToNextX < posToNextY)
				{
					posToNextX += distToNextX;
					player.x += stepX;
					side = 0;
				}
				else
				{
					posToNextY += distToNextY;
					player.y += stepY;
					side = 1;
				}

				/* check if ray hit a wall */
				if (maze[player.x][player.y] > 0)
					hit = 1;
			}

			/* calculate distance projected in camera direction */
			if (side == 0)
				distToWall = (player.x - rayPosX + (1 - stepX) / 2) / rayDirX;
			else
				distToWall = (player.y - rayPosY + (1 - stepY) / 2) / rayDirY;

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

		/* timing for input */
		oldTime = time;
		time = SDL_GetTicks();
		frameTime = (time - oldTime) / 1000.0;

		/* update screen */
		SDL_RenderPresent(renderer);

		/* speed modifiers */
		moveSpeed = frameTime * 5.0; /* constant in squares/second */
		rotateSpeed = frameTime * 3.0; /* constant in radians/second */

		keystate = SDL_GetKeyboardState(NULL);

		/* move forward if no wall in front */
		if (keystate[SDL_SCANCODE_W])
		{
			if (!maze[(int)(posX + dirX * moveSpeed)][(int)(posY)])
				posX += dirX * moveSpeed;
			if (!maze[(int)(posX)][(int)(posY + dirY * moveSpeed)])
				posY += dirY * moveSpeed;
		}

		/* move backward if no wall behind */
		if (keystate[SDL_SCANCODE_S])
		{
			if (!maze[(int)(posX - dirX * moveSpeed)][(int)(posY)])
				posX -= dirX * moveSpeed;
			if (!maze[(int)(posX)][(int)(posY - dirY * moveSpeed)])
				posY -= dirY * moveSpeed;
		}

		/* rotate left */
		if (keystate[SDL_SCANCODE_D])
		{
			/* rotate camera direction */
			oldDirX = dirX;
			dirX = dirX * cos(rotateSpeed) - dirY * sin(rotateSpeed);
			dirY = oldDirX * sin(rotateSpeed) + dirY * cos(rotateSpeed);

			/* rotate camera plane */
			oldPlaneX = planeX;
			planeX = planeX * cos(rotateSpeed) - planeY * sin(rotateSpeed);
			planeY = oldPlaneX * sin(rotateSpeed) + planeY * cos(rotateSpeed);
		}

		/* rotate right */
		if (keystate[SDL_SCANCODE_A])
		{
			/* rotate camera direction */
			oldDirX = dirX;
			dirX = dirX * cos(-rotateSpeed) - dirY * sin(-rotateSpeed);
			dirY = oldDirX * sin(-rotateSpeed) + dirY * cos(-rotateSpeed);

			/* rotate camera plane */
			oldPlaneX = planeX;
			planeX = planeX * cos(-rotateSpeed) - planeY * sin(-rotateSpeed);
			planeY = oldPlaneX * sin(-rotateSpeed) + planeY * cos(-rotateSpeed);
		}
	}

	/* close renderer and window */
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	freeMap(maze);

	return (0);
}
