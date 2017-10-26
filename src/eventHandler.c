#include "maze.h"

/**
 * eventHandler -
 *
 */
bool eventHandler(int **maze)
{
	double oldDirX, oldPlaneX; /* previous X direction vector and X plane */

	double moveSpeed; /* constant in squares/second */
	double rotateSpeed; /* constant in radians/second */

	double oldTime; /*time of previous frame */
	double frameTime; /* time the frame has taken in seconds */

	const uint8_t *keystate; /* current key state */

	SDL_Event event; /* event listener */

	bool quit; /* main loop flag */

	/* timing for input */
	oldTime = time;
	time = SDL_GetTicks();
	frameTime = (time - oldTime) / 1000.0;

	/* speed modifiers */
	moveSpeed = frameTime * 5.0; /* constant in squares/second */
	rotateSpeed = frameTime * 3.0; /* constant in radians/second */

	keystate = SDL_GetKeyboardState(NULL);

	/* check if user exits */
	while (SDL_PollEvent(&event) != 0)
	{
		/* user requests quit */
		if (event.type == SDL_QUIT || keystate[SDL_SCANCODE_ESCAPE])
			quit = true;
	}

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

	return (quit);
}
