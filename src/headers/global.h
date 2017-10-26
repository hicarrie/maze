#ifndef GLOBAL_H
#define GLOBAL_H

/* render window */
extern SDL_Window *window;

/* window renderer */
extern SDL_Renderer *renderer;

/* X and Y start position */
extern point_t pos;

/* initial direction vector */
extern point_t dir;

/* camera plane */
extern point_t plane;

/* time of current frame */
extern double time;

#endif /* GLOBAL_H */
