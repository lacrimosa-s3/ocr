#ifndef PIXEL_OPERATIONS_H_
#define PIXEL_OPERATIONS_H_

#include <stdlib.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#include <gmodule.h>

Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y);
void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);
void update_surface(SDL_Surface* screen, SDL_Surface* image);
void imageCrop (SDL_Surface* img , int leftBorder , int topBorder , int rightBorder, int bottomBorder, const char* label);
SDL_Surface* rotate(SDL_Surface *img , float theta);



struct coord
{
  ssize_t x;
  ssize_t y;
};


struct symbol
{
  GArray *coordinates;    
  struct coord miny;     
  struct coord maxy;     
  struct coord minx;
  struct coord maxx;
  float area;
  
};
/*
struct Jarray_symbol
{
  struct symbol * array;
  size_t len;
  size_t capacity;
};

struct Jarray_coord
{
  struct coord * array;
  size_t len;
  size_t capacity;
};
*/

int create_symbol (SDL_Surface *img, struct coord pos, int *marks_board , struct symbol current_symbol);
int *generate_marks_board(int height, int width);
void create_symbols (SDL_Surface *img,GArray *symbols);
void print_symbols (GArray * coords);
void determine_symbol_borders (struct symbol *current_symbol);
void determine_symbol_area (struct symbol *current_symbol);
struct symbol largest_area (GArray* symbols);

#endif
