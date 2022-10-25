#ifndef TOTO
#define TOTO

#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"



void init_sdl();
SDL_Surface* load_image(char *path);
SDL_Surface* display_image(SDL_Surface *img);
void wait_for_keypressed();
SDL_Surface* createSurface(int height, int width);
void freeSDL(SDL_Surface* image);
#endif
