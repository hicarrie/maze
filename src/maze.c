#include "maze.h"

/* world map */
int worldMap[MAP_WIDTH][MAP_HEIGHT]=
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
};

/* render window (global) */
SDL_Window *window = NULL;

/* window renderer (global) */
SDL_Renderer *renderer = NULL;

/**
 * init - initializes SDL, window, and renderer
 * Return: True on success, False on failure
 */
bool init(void)
{
	/* initialization flag */
	bool success = true;

	/* initialize SDL */
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n",
		       SDL_GetError());
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
			renderer =
				SDL_CreateRenderer(window, -1,
						   SDL_RENDERER_ACCELERATED);
			if (renderer == NULL)
			{
				printf("Renderer could not be created!");
				printf("SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
				SDL_SetRenderDrawColor(renderer, 0x31, 0x5D, 0x5F, 0xFF);
		}
	}
	printf("Window successfully created\n");
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

	double sideDistX; /* length of ray from current position to next X side */
	double sideDistY; /* length of ray from current position to next Y side */

	double deltaDistX; /* length of ray from X side to next X side */
	double deltaDistY; /* length of ray from Y side to next Y side */
	double perpWallDist;

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

	SDL_Event event;
	bool quit; /* main loop flag */

	int x; /* x column counter for raycasting */

	posX = 22;
	posY = 12;
	dirX = -1;
	dirY = 0;
	planeX = 0;
	planeY = 0.66;
	time = 0;
	oldTime = 0;
	quit = false;

	/* start SDL and create window */
	init();

	while (!quit)
	{
		/* clear screen */
		SDL_SetRenderDrawColor(renderer, 0x31, 0x5D, 0x5F, 0xFF);
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

			printf("Raycast: %d\n", x);
			/* calculate ray position and direction */
			cameraX = 2 * x / (double)(SCREEN_WIDTH) - 1;
			rayPosX = posX;
			rayPosY = posY;
			rayDirX = dirX + planeX * cameraX;
			rayDirY = dirY + planeY * cameraX;

			mapX = (int)(rayPosX);
			mapY = (int)(rayPosY);

			/* sine / cosine functions are cheaper than sqrt - convert? */
			deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
			deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY));

			hit = 0;

			/* calculate step and initial sideDist */
			if (rayDirX < 0)
			{
				stepX = -1;
				sideDistX = (rayPosX - mapX) * deltaDistX;
			}
			else
			{
				stepX = 1;
				sideDistX = (mapX + 1.0 - rayPosX) * deltaDistX;
			}

			if (rayDirY < 0)
			{
				stepY = -1;
				sideDistY = (rayPosY - mapY) * deltaDistY;
			}
			else
			{
				stepY = 1;
				sideDistY = (mapY + 1.0 - rayPosY) * deltaDistY;
			}

			while (hit == 0)
			{
				printf("Raycast moving to next map square\n");
				/* move to next map square in X or Y direction */
				if (sideDistX < sideDistY)
				{
					sideDistX += deltaDistX;
					mapX += stepX;
					side = 0;
				}
				else
				{
					sideDistY += deltaDistY;
					mapY += stepY;
					side = 1;
				}

				/* check if ray hit a wall */
				if (worldMap[mapX][mapY] > 0)
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

			/* wall colors */
			/* SDL_Color color; */
			/* switch (worldMap[mapX][mapY]) */
			/* { */
			/* case 1: color.r = 255; break; */
			/* case 2: color.g = 255; break; */
			/* case 3: color.b = 255; break; */
			/* case 4: color.r, color.g, color.b = 255; break; */
			/* default: color.g, color.b = 255; break; */
			/* } */

			if (side == 0)
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
			else if (side == 1)
				SDL_SetRenderDrawColor(renderer, 130, 130, 130, SDL_ALPHA_OPAQUE);

			/* draw pixels of wall slice as a vertical line */
			printf("Before drawing line\n");
			SDL_RenderDrawLine(renderer, x, drawStart, x, drawEnd);
			printf("After drawing line\n");

			/* update screen */
			SDL_RenderPresent(renderer);
		}

		/* /\* timing for input and FPS counter *\/ */
		/* oldTime = time; */
		/* time = getTicks(); */
		/* frameTime = (time - oldTime) / 1000.0; */
		/* print(1.0 / frameTime); /\* FPS counter *\/ */
		/* redraw(); */
		/* cls(); */

		/* /\* speed modifiers *\/ */
		/* moveSpeed = frameTime * 5.0; /\* constant in squares/second *\/ */
		/* rotateSpeed = frameTime = 3.0; /\* constant in radians/second *\/ */
		/* readKeys(); */

		/* /\* move forward if no wall in front *\/ */
		/* if (event.type == SDL_KEYDOWN && event.key.keysym) */
		/* { */
		/* 	if (!worldMap[(int)(posX + dirX * moveSpeed)][(int)(posY)]) */
		/* 		posX += dirX * moveSpeed; */
		/* 	if (!worldMap[(int)(posX)][(int)(posY + dirY * moveSpeed)]) */
		/* 		posY += dirY * moveSpeed; */
		/* } */

		/* /\* move backward if no wall behind *\/ */
		/* if (keyDown(SDLK_DOWN)) */
		/* { */
		/* 	if (!worldMap[(int)(posX - dirX * moveSpeed)][(int)(posY)]) */
		/* 		posX -= dirX * moveSpeed; */
		/* 	if (!worldMap[(int)(posX)][(int)(posY - dirY * moveSpeed)]) */
		/* 		posY -= dirY * moveSpeed; */
		/* } */

		/* /\* rotate left *\/ */
		/* if (keyDown(SDLK_LEFT)) */
		/* { */
		/* 	/\* rotate camera direction *\/ */
		/* 	oldDirX = dirX; */
		/* 	dirX = dirX * cos(rotateSpeed) - dirY * sin(rotateSpeed); */
		/* 	dirY = oldDirX * sin(rotateSpeed) + dirY * cos(rotateSpeed); */

		/* 	/\* rotate camera plane *\/ */
		/* 	oldPlaneX = planeX; */
		/* 	planeX = planeX * cos(rotateSpeed) - planeY * sin(rotateSpeed); */
		/* 	planeY = oldPlaneX * sin(rotateSpeed) + planeY * cos(rotateSpeed); */
		/* } */

		/* /\* rotate right *\/ */
		/* if (keyDown(SDLK_RIGHT)) */
		/* { */
		/* 	/\* rotate camera direction *\/ */
		/* 	oldDirX = dirX; */
		/* 	dirX = dirX * cos(-rotateSpeed) - dirY * sin(-rotateSpeed); */
		/* 	dirY = oldDirX * sin(-rotateSpeed) + dirY * cos(-rotateSpeed); */

		/* 	/\* rotate camera plane *\/ */
		/* 	oldPlaneX = planeX; */
		/* 	planeX = planeX * cos(-rotateSpeed) - planeY * sin(-rotateSpeed); */
		/* 	planeY = oldPlaneX * sin(-rotateSpeed) + planeY * cos(-rotateSpeed); */
		/* } */
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return (0);
}
