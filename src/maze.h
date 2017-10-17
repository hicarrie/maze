#ifndef MAZE_H
#define MAZE_H

/* header files */
#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

/* macros */
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 800
#define TEX_WIDTH 64
#define TEX_HEIGHT 64
#define MAP_WIDTH 24
#define MAP_HEIGHT 24

bool init(void);

#endif /* MAZE_H */
