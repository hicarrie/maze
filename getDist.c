#include "maze.h"

/**
 * getVertDist - gets distance to wall by X intersections
 * @rayAngle: angle of ray
 * Return: distance to wall
 */
double getVertDist(double rayAngle, double cameraX, double cameraY, int **map, int mapX, int mapY)
{
	double rayX;
	double stepX;
	double stepY;
	double distToNextX;
	double distToNextY;
	double distToWall;

	bool hit;

	rayX = cos(rayAngle);

	if (rayX < 0)
	{
		stepX = -1;
		distToNextX = cameraX - (int)cameraX * stepX;
	}
	else if (rayX > 0)
	{
		stepX = 1;
		distToNextX = ((int)(cameraX + 1)) - cameraX;
	}

	stepY = (stepX * tan(rayAngle) * -1);
	distToNextY = abs(distToNextX) * stepY;

	hit = false;

	if (map[(int)(mapX)][(int)(mapY)] > 0)
		hit = true;

	printf("Hit: %d\n", hit);
	printf("MapX: %d\n", mapX);
	printf("MapY: %d\n", mapY);

	mapX += distToNextX;
	mapY += distToNextY;

	if (map[(int)(mapX)][(int)(mapY)] > 0)
		hit = true;
	else
	{
		while (!hit && mapX > 0 && mapY > 0)
		{
			mapX += stepX;
			mapY += stepY;
			if (map[(int)(mapX)][(int)(mapY)] > 0)
				hit = true;
		}
	}

	mapX = abs(mapX - cameraX);
	mapY = abs(mapY - cameraY);

	distToWall = sqrt((mapX * mapX) + (mapY * mapY));

	return (distToWall);
}
