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
#include "structs.h" /* struct definitions */
#include "global.h" /* global variables */

/* macros */
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 800
#define TEX_WIDTH 64
#define TEX_HEIGHT 64
#define MAP_WIDTH 24
#define MAP_HEIGHT 24
#define MAP_CHAR_COUNT 1275

/* function declarations */
int **parseMap(const char *filename, int **map);

bool initSDL(void);
void closeSDL(void);

void inputHandler(int **maze);
bool quit(void);

void renderBG(void);
void raycaster(int **maze);

void freeMap(int **map);

#endif /* MAZE_H */
