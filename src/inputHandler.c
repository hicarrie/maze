#include "maze.h"

/**
 * inputHandler - checks user input for movement
 * @maze: 2D array defining maze map
 * Return: void
 */
void inputHandler(int **maze)
{
	const uint8_t *keystate; /* current key state */
	double oldTime; /* time of previous frame */
	double frameTime; /* time the frame has taken in seconds */
	double oldDirX, oldPlaneX; /* previous dirX and planeX */
	double moveSpeed; /* move speed modifier */
	double rotateSpeed; /* rotate speed modifier */

	keystate = SDL_GetKeyboardState(NULL);
	oldTime = time;
	time = SDL_GetTicks();
	frameTime = (time - oldTime) / 1000.0;
	moveSpeed = frameTime * 5.0;
	rotateSpeed = frameTime * 3.0;

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

	/* strafe left */
	if (keystate[SDL_SCANCODE_Q])
	{
		if (!maze[(int)(posX - planeX * moveSpeed)][(int)(posY)])
			posX -= planeX * moveSpeed;
		if (!maze[(int)(posX)][(int)(posY - planeY * moveSpeed)])
			posY -= planeY * moveSpeed;
	}

	/* strafe right */
	if (keystate[SDL_SCANCODE_E])
	{
		if (!maze[(int)(posX + planeX * moveSpeed)][(int)(posY)])
			posX += planeX * moveSpeed;
		if (!maze[(int)(posX)][(int)(posY + planeY * moveSpeed)])
			posY += planeY * moveSpeed;
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

/**
 * quit - checks if user quits
 * Return: True if user quits, else False
 */
bool quit(void)
{
	SDL_Event event; /* event listener */
	bool quit;

	quit = false;
	while (SDL_PollEvent(&event) != 0)
	{
		/* if window's close button is pressed */
		if (event.type == SDL_QUIT)
			quit = true;

		/* if ESC is pressed */
		if (event.type == SDL_KEYDOWN &&
		    event.key.keysym.sym == SDLK_ESCAPE)
			quit = true;
	}

	return (quit);
}
