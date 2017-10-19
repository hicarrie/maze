#include "maze.h"

/* int map[MAP_WIDTH][MAP_HEIGHT] =
{
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
  }; */

/* render window */
SDL_Window *window = NULL;

/* window renderer */
SDL_Renderer *renderer = NULL;

/**
 * init - initializes SDL, window, and renderer
 * Return: True on success, False on failure
 */
bool init(void)
{
	bool success = true;

	/* initialize SDL */
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		/* create window */
		window = SDL_CreateWindow("Maze",
					   SDL_WINDOWPOS_UNDEFINED,
					   SDL_WINDOWPOS_UNDEFINED,
					   SCREEN_WIDTH, SCREEN_HEIGHT,
					   SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n",
			       SDL_GetError());
			success = false;
		}
		else
		{
			renderer = SDL_CreateRenderer(window, -1,
						      SDL_RENDERER_ACCELERATED);
			if (renderer == NULL)
			{
				printf("Renderer could not be created!");
				printf("SDL Error: %s\n", SDL_GetError());
				success = false;
			}
		}
	}
	return (success);
}


/**
 * main - renders maze
 * @argc - number of arguments
 * @argv - pointer to array of arguments
 * Return: 0 on success
 */
int main(int argc, char *argv[])
{
	int **map; /* 2D array defining map */

	double posX, posY; /* X and Y start position */
	double dirX, dirY; /* initial direction vector */
	double planeX, planeY; /* camera plane */
	double oldDirX, oldPlaneX; /* previous X direction vector and X plane */

	double cameraX; /* X coordinate in camera space */
	double rayPosX; /* X coordinate of ray position */
	double rayPosY; /* Y coordinate of ray position */
	double rayDirX; /* direction of X coordinate of ray position */
	double rayDirY; /* direction of Y coordinate of ray position */

	int mapX; /* X coordinate of box of map player is currently in */
	int mapY; /* Y coordinate of box of map player is currently in */

	double posToNextX; /* length of ray from current position to next X side */
	double posToNextY; /* length of ray from current position to next Y side */

	double lengthToNextX; /* length of ray from X side to next X side */
	double lengthToNextY; /* length of ray from Y side to next Y side */
	double perpWallDist; /* used to calculate length of ray */

	int stepX; /* X direction to step in */
	int stepY; /* Y direction to step in */

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

	SDL_Event event;
	bool quit; /* main loop flag */

	int x; /* column counter for raycasting */

	posX = 1;
	posY = 12;
	dirX = 1;
	dirY = -0.66;
	planeX = 0;
	planeY = 0.66;
	time = 0;
	oldTime = 0;
	quit = false;

	/* parse map file */
	map = parseMap(argv[1], map);

	/* start SDL and create window */
	init();

	while (!quit)
	{
		/* clear screen */
		SDL_SetRenderDrawColor(renderer, 0x1E, 0x29, 0x34, 0xFF);
		SDL_RenderClear(renderer);

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

			/* grid position on map */
			mapX = (int)(rayPosX);
			mapY = (int)(rayPosY);

			/* sine / cosine functions are cheaper than sqrt - convert? */
			lengthToNextX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
			lengthToNextY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY));

			hit = 0;

			/* calculate step and initial distance from position to X or Y */
			if (rayDirX < 0)
			{
				stepX = -1;
				posToNextX = (rayPosX - mapX) * lengthToNextX;
			}
			else
			{
				stepX = 1;
				posToNextX = (mapX + 1.0 - rayPosX) * lengthToNextX;
			}

			if (rayDirY < 0)
			{
				stepY = -1;
				posToNextY = (rayPosY - mapY) * lengthToNextY;
			}
			else
			{
				stepY = 1;
				posToNextY = (mapY + 1.0 - rayPosY) * lengthToNextY;
			}

			while (hit == 0)
			{
				/* move to next map square in X or Y direction */
				if (posToNextX < posToNextY)
				{
					posToNextX += lengthToNextX;
					mapX += stepX;
					side = 0;
				}
				else
				{
					posToNextY += lengthToNextY;
					mapY += stepY;
					side = 1;
				}

				/* check if ray hit a wall */
				if (map[mapX][mapY] > 0)
					hit = 1;
			}

			/* calculate distance projected in camera direction */
			if (side == 0)
				perpWallDist = (mapX - rayPosX + (1 - stepX) / 2) / rayDirX;
			else
				perpWallDist = (mapY - rayPosY + (1 - stepY) / 2) / rayDirY;

			/* calculate height of wall slice to draw on screen */
			sliceHeight = (int)(SCREEN_HEIGHT / perpWallDist);

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
			if (!map[(int)(posX + dirX * moveSpeed)][(int)(posY)])
				posX += dirX * moveSpeed;
			if (!map[(int)(posX)][(int)(posY + dirY * moveSpeed)])
				posY += dirY * moveSpeed;
		}

		/* move backward if no wall behind */
		if (keystate[SDL_SCANCODE_S])
		{
			if (!map[(int)(posX - dirX * moveSpeed)][(int)(posY)])
				posX -= dirX * moveSpeed;
			if (!map[(int)(posX)][(int)(posY - dirY * moveSpeed)])
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

	return (0);
}
