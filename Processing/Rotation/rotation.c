#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL_tools.h"

static Uint32 ReadPix(SDL_Surface* surface, int x, int y);

static void WritePix(SDL_Surface* surface, int x, int y, Uint32 pixel);

inline Uint32 ReadPix(SDL_Surface* surface, int x, int y)
{
  int bpp = surface->format->BytesPerPixel;

  Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

  switch(bpp)
    {
    case 1:
      return *p;

    case 2:
      return *(Uint16 *)p;

    case 3:
      if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
	return p[0] << 16 | p[1] << 8 | p[2];
      else
	return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
      return *(Uint32 *)p;

    default:
      return 0;
    }
}


inline void WritePix(SDL_Surface* surface, int x, int y, Uint32 pixel)
{
  int bpp = surface->format->BytesPerPixel;

  Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

  switch(bpp)
    {
    case 1:
      *p = pixel;
      break;

    case 2:
      *(Uint16 *)p = pixel;
      break;

    case 3:
      if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
	{
      p[0] = (pixel >> 16) & 0xff;
	  p[1] = (pixel >> 8) & 0xff;
	  p[2] = pixel & 0xff;
	  
    }
      else
	{
      p[0] = pixel & 0xff;
	  p[1] = (pixel >> 8) & 0xff;
	  p[2] = (pixel >> 16) & 0xff;
      	  
    }
      break;

    case 4:
      *(Uint32 *)p = pixel;
      int tmp = p[2];
      p[2] = p[0];
      p[0] = tmp;
         break;
    }

}

SDL_Surface* createSurface(int width, int height)
{
    SDL_Surface *surface;
    Uint32 rmask, gmask, bmask, amask;

    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;

    surface = SDL_CreateRGBSurface(0, width, height, 32, rmask, gmask, bmask, amask);
    return surface;
}

void Rotate(char source[], char result[], double angle)
{
    int x0, y0, x1, y1, x2, y2;
    Uint32 color;
    double angle_rad;

    angle_rad = angle * (M_PI / 180.0);

    SDL_Surface *origin = load_image(source);

    SDL_Surface *dest;

    double theta = fabs(angle_rad);
    while (theta > M_PI/2)
    {
        theta -= M_PI/2;
    }
    int new_width = (origin->w) * cos(theta) + (origin->h) * sin(theta);
    int new_height = (origin->w) * sin(theta) + (origin->h) * cos(theta);
    dest = createSurface(new_width, new_height);

    //printf("width: %d, height: %d\n", new_width, new_height);

    init_sdl();
   
    x0 = (origin->w / 2);
    y0 = (origin->h / 2);

    for (x2 = 0; x2 < new_width; x2++)
    {
        for (y2 = 0; y2 < new_height; y2++)
        {
            x1 = (x2 - new_width/2) * cos(angle_rad) - (y2 - new_height/2) * sin(angle_rad) + x0;
            y1 = (x2 - new_width/2) * sin(angle_rad) + (y2 - new_height/2) * cos(angle_rad) + y0;
            
            if ((x1 >= 0) && (x1 < origin->w) && (y1 >= 0) && (y1 < origin->h))
            {
                color = ReadPix(origin, x1, y1);
                color += 0xff000000;
                WritePix(dest, x2, y2, color);
            }
            else
            {
                WritePix(dest, x2, y2, 0xffffffff);
            }

        }
    }
    
    SDL_UpdateRect(dest, 0, 0, new_width, new_height);
    
    SDL_SaveBMP(dest, result);
    SDL_Quit();
}


int main()
{
    double angle;
    printf("Enter a rotation degree : ");
    scanf("%lf", &angle);

    Rotate("image.jpeg", "result.jpeg", angle);

    printf("Image rotated by %f degrees \n", angle);

    return 0;

}

