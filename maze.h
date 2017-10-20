#ifndef MAZE_H
#define MAZE_H

/* header files */
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>
#include "global.h" /* global SDL variables */

/* macros */
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 800
#define TEX_WIDTH 64
#define TEX_HEIGHT 64
#define MAP_WIDTH 24
#define MAP_HEIGHT 24
#define MAP_CHAR_COUNT 1275

bool initSDL(void);
int **parseMap(const char *filename, int **map);

/* utility functions */
void freeMap(int **map);

#endif /* MAZE_H */
