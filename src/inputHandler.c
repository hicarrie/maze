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
	double oldDirX, oldPlaneX; /* previous dir.x and plane.x */
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
		if (!maze[(int)(pos.x + dir.x * moveSpeed)][(int)(pos.y)])
			pos.x += dir.x * moveSpeed;
		if (!maze[(int)(pos.x)][(int)(pos.y + dir.y * moveSpeed)])
			pos.y += dir.y * moveSpeed;
	}

	/* move backward if no wall behind */
	if (keystate[SDL_SCANCODE_S])
	{
		if (!maze[(int)(pos.x - dir.x * moveSpeed)][(int)(pos.y)])
			pos.x -= dir.x * moveSpeed;
		if (!maze[(int)(pos.x)][(int)(pos.y - dir.y * moveSpeed)])
			pos.y -= dir.y * moveSpeed;
	}

	/* strafe left */
	if (keystate[SDL_SCANCODE_Q])
	{
		if (!maze[(int)(pos.x - plane.x * moveSpeed)][(int)(pos.y)])
			pos.x -= plane.x * moveSpeed;
		if (!maze[(int)(pos.x)][(int)(pos.y - plane.y * moveSpeed)])
			pos.y -= plane.y * moveSpeed;
	}

	/* strafe right */
	if (keystate[SDL_SCANCODE_E])
	{
		if (!maze[(int)(pos.x + plane.x * moveSpeed)][(int)(pos.y)])
			pos.x += plane.x * moveSpeed;
		if (!maze[(int)(pos.x)][(int)(pos.y + plane.y * moveSpeed)])
			pos.y += plane.y * moveSpeed;
	}

	/* rotate left */
	if (keystate[SDL_SCANCODE_D])
	{
		/* rotate camera direction */
		oldDirX = dir.x;
		dir.x = dir.x * cos(rotateSpeed) - dir.y * sin(rotateSpeed);
		dir.y = oldDirX * sin(rotateSpeed) + dir.y * cos(rotateSpeed);

		/* rotate camera plane */
		oldPlaneX = plane.x;
		plane.x = plane.x * cos(rotateSpeed) - plane.y * sin(rotateSpeed);
		plane.y = oldPlaneX * sin(rotateSpeed) + plane.y * cos(rotateSpeed);
	}

	/* rotate right */
	if (keystate[SDL_SCANCODE_A])
	{
		/* rotate camera direction */
		oldDirX = dir.x;
		dir.x = dir.x * cos(-rotateSpeed) - dir.y * sin(-rotateSpeed);
		dir.y = oldDirX * sin(-rotateSpeed) + dir.y * cos(-rotateSpeed);

		/* rotate camera plane */
		oldPlaneX = plane.x;
		plane.x = plane.x * cos(-rotateSpeed) - plane.y * sin(-rotateSpeed);
		plane.y = oldPlaneX * sin(-rotateSpeed) + plane.y * cos(-rotateSpeed);
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