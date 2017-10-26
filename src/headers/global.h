#ifndef GLOBAL_H
#define GLOBAL_H

/* render window */
extern SDL_Window *window;

/* window renderer */
extern SDL_Renderer *renderer;

/* X and Y start position */
extern double posX, posY;

/* initial direction vector */
extern double dirX, dirY;

/* camera plane */
extern double planeX, planeY;

/* time of current frame */
extern double time;

#endif /* GLOBAL_H */
