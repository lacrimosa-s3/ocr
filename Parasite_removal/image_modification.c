// Simple get/put pixel for SDL
// Inspired by code from SDL documentation
// (http://www.libsdl.org/release/SDL-1.2.15/docs/html/guidevideo.html)

#include <gmodule.h>

#include <err.h>
#include "image_modification.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#include <math.h>
#include <stdlib.h>


#include "display.h"

static inline
Uint8* pixel_ref(SDL_Surface *surf, unsigned x, unsigned y)
{
    int bpp = surf->format->BytesPerPixel;
    return (Uint8*)surf->pixels + y * surf->pitch + x * bpp;
}

Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y)
{
    Uint8 *p = pixel_ref(surface, x, y);

    switch (surface->format->BytesPerPixel)
    {
        case 1:
            return *p;

        case 2:
            return *(Uint16 *)p;

        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;

        case 4:
            return *(Uint32 *)p;
    }

    return 0;
}

void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel)
{
    Uint8 *p = pixel_ref(surface, x, y);

    switch(surface->format->BytesPerPixel)
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
            break;
    }
}

void imageCrop (SDL_Surface* img , int leftBorder , int topBorder , int rightBorder, int bottomBorder , const char* label)
{
  SDL_Surface* image = img;
  SDL_Surface* surface;
  surface = createSurface(bottomBorder-topBorder,rightBorder-leftBorder); //creation of empty surface with proper dimensions
  for (int i= topBorder; i< bottomBorder ; i++)
    {
      for(int j=leftBorder; j < rightBorder ; j++)
	{
	  Uint32 pixel = get_pixel(image, j,i);                           //copying of pixel
	  put_pixel(surface,j-leftBorder,i-topBorder, pixel);             // pasting onto neqw surface with proper dimensions.
	}
    }
  SDL_SaveBMP(surface,label);                                             //saving of cropped image
  
}


SDL_Surface* rotate(SDL_Surface *img , float theta)
{
    for (int i = 0; i < img->w; i++)
    {
        for (int j = 0; j < img->h; j++)
        {
            int x = i*cos(theta) - j*sin(theta);
            int y = i*sin(theta) + j*cos(theta);

            if ((x >= 0) && (x < img->w) && (y >= 0) && (y < img->h))
            {
                img[i*img->w + j] = img[x*img->w + y];
            }
        }
    }
    return img;
}

void update_surface(SDL_Surface* screen, SDL_Surface* image)
{
    if (SDL_BlitSurface(image, NULL, screen, NULL) < 0)
        warnx("BlitSurface error: %s\n", SDL_GetError());

    SDL_UpdateRect(screen, 0, 0, image->w, image->h);
}


int is_black (SDL_Surface *img,struct coord pos)
{
    
  Uint32 pixel = get_pixel(img, pos.x,pos.y);
  Uint8 r, g, b;
  SDL_GetRGB(pixel, img->format, &r, &g, &b);
  return (!(r > 25 || g > 25 || b > 25));
  
}

GArray* generate_neighbours (struct coord pos, int height, int width)
{
  GArray *neighbours = g_array_new(0,1,sizeof(struct coord));
  for (int i = -1 ; i < 2 ; i++)
    {
      for(int j = -1 ; j < 2 ; j++)
	{
	  if ((pos.x+i) >= 0 && (pos.y+j) >=0 && (pos.x+i) < width && (pos.y+j) < height)
	    {
	      struct coord neighbour = {.x=pos.x+i
					,.y=pos.y+j,};

	      neighbours = g_array_append_val(neighbours,neighbour);
	    }
	}
    }
  return neighbours;
}

int create_symbol (SDL_Surface *img, struct coord pos, int *marks_board , struct symbol current_symbol)
{

  
  int height = img -> h;
  int width = img -> w;
  
  if (!is_black(img,pos))
    {
      return 0;
    }
  GQueue * file = g_queue_new();
  struct coord *p =malloc(sizeof(struct coord));
  p->x = pos.x;
  p->y = pos.y;
  g_queue_push_head (file,p);

  int *mark = (marks_board + pos.y*width + pos.x);
  *mark += 1;
  while (!g_queue_is_empty (file))
    {
      struct coord *s = g_queue_pop_tail(file);
      //printf("(%ld,%ld) \n",s->x,s->y);
      current_symbol.coordinates = g_array_append_val(current_symbol.coordinates, *s);


      GArray * neighbours = generate_neighbours(*s,height,width);
      free(s);
      for (int i = 0; i< neighbours->len;i++)
	{
	  struct coord neighbour = g_array_index(neighbours,struct coord,i);
	  struct coord* a = malloc (sizeof(struct coord));
	  a->x = neighbour.x;
	  a->y = neighbour.y;
	  int *mark = (marks_board + neighbour.y*width + neighbour.x);
	  if ((*mark==0) && is_black(img,neighbour))
	    {
	      *mark += 1;
	      
	      //printf("(%ld,%ld) \n",neighbour.x,neighbour.y);
	      
	      g_queue_push_head (file,a);
	    }
	  else
	    {
	      free(a);
	    }
	}
      g_array_free(neighbours,TRUE);
      
    }
  g_queue_free(file);
  return 1; 
}


      GArray * neighbours = generate_neighbours(*s,height,width);
      free(s);
      for (int i = 0; i< neighbours->len;i++)
	{
	  struct coord neighbour = g_array_index(neighbours,struct coord,i);
	  struct coord* a = malloc (sizeof(struct coord));
	  a->x = neighbour.x;
	  a->y = neighbour.y;
	  int *mark = (marks_board + neighbour.y*width + neighbour.x);
	  if ((*mark==0) && !is_black(img,neighbour))
	    {
	      *mark += 1;
	      
	      //printf("(%ld,%ld) \n",neighbour.x,neighbour.y);
	      
	      g_queue_push_head (file,a);
	    }
	  else
	    {
	      free(a);
	    }
	}
      g_array_free(neighbours,TRUE);
      
    }
  g_queue_free(file);
  return 1; 
}


SDL_Surface * grid_eraser (SDL_Surface * img , struct symbol grid , const char * label)
{
  GArray * coordinates = grid.coordinates;
  Uint32 white = SDL_MapRGB(img->format, 255, 255, 255);
  for (int i = 0 ; i < coordinates->len ; i++)
    {
      struct coord coords = g_array_index(coordinates,struct coord,i);
      put_pixel(img,coords.x,coords.y,white);      
    }
  SDL_SaveBMP(img,label); 
}


int* generate_marks_board(int height, int width)
{
  int *board = malloc(height*(width*sizeof(int)));
  for (int i = 0; i < height; i++)
    {
      for (int j = 0; j < width; j++)
	{
	  *(board + (j+ i*width)) = 0; 
	}      
      
      //marks_print(board);
    }
  //print_board(board, height,width);
	
 
  return board;
}

void print_board (int *board, int height, int width)
            {
    for (int i = 0; i< height ; i++)
    {
      printf("%d \n", i);
      for (int j = 0; j< width; j++)
	{
	  printf("%d ", *(board + (j+ i*width)));
	}
      printf("\n");
    }
}

void print_symbols (GArray * coords)
{
  for (int j = 0; j< coords-> len; j++)
    {
      struct coord current_coord = g_array_index(coords,struct coord, j);
      printf("(%ld,%ld) |",current_coord.x,current_coord.y);
    }
  //printf("\n");
}
void determine_symbol_borders (struct symbol* current_symbol)
{
  GArray * current_coords = current_symbol->coordinates;

  struct coord initial = g_array_index (current_coords, struct coord, 0);
  current_symbol->miny = initial;
  current_symbol->maxy = initial;
  current_symbol->maxx = initial;
  current_symbol->minx = initial;
  for(int i = 0; i<(current_coords->len); i++)
    {
      struct coord pos = g_array_index (current_coords, struct coord, i);
      struct coord miny_t = current_symbol->miny;
      struct coord maxy_t = current_symbol->maxy;
      struct coord minx_t = current_symbol->minx;
      struct coord maxx_t = current_symbol->maxx;

      if (miny_t.y > pos.y)
	{
	  current_symbol->miny = pos;
	}
      if (minx_t.x > pos.x)
	{
	  current_symbol->minx = pos;
	}
      if (maxy_t.y < pos.y)
	{
	  current_symbol->maxy = pos;
	}
      if (maxx_t.x < pos.x)
	{
	  current_symbol->maxx = pos;
	}
    }
}

void determine_symbol_area (struct symbol* current_symbol)
{
  if ((current_symbol->minx.x != current_symbol->miny.x) || (current_symbol->minx.y != current_symbol->miny.y))
    {
      float height_x = current_symbol->maxy.x - current_symbol->minx.x;
      float height_y = current_symbol->maxy.y - current_symbol->minx.y;
      
      float height =  sqrtf((height_x*height_x) + (height_y*height_y));

      
      float width_x = current_symbol->miny.x - current_symbol->minx.x;
      float width_y = current_symbol->minx.y - current_symbol->miny.y;
 
      float width = sqrtf((width_x*width_x) + (width_y*width_y));
 

      current_symbol->area = width * height;

    }
  else
    {
      float height = current_symbol->maxy.y - current_symbol -> miny.y;
      float width = current_symbol -> maxx.x - current_symbol -> minx.x;
      current_symbol->area = width * height;
    }
        
  
}

struct symbol largest_area (GArray* symbols)
{
  int index = 0;
  float max_area = g_array_index(symbols,struct symbol, 0).area;
  for (int i = 1; i < symbols -> len ; i++)
    {
      struct symbol current_symbol = g_array_index(symbols,struct symbol,i);
      if (current_symbol.area > max_area)
	{
	  index = i;
	  max_area = g_array_index(symbols,struct symbol,i).area;
	}
      //printf("%f \n",current_symbol.area);
    }
  return( g_array_index(symbols,struct symbol,index));
}

/*void draw_symbols (struct symbol current_symbol)
{
  
}*/


//A small distinction in function name with an "s", rigor needs to be applied, We wouldn't be programmers if we weren't rigorous (/satire)
void create_symbols (SDL_Surface *img,GArray *symbols)
{
  int height = img -> h;
  int width = img -> w;

  
  int *marks_board = generate_marks_board(height,width);
  for (int i = 0 ; i < height ; i++)
    {
      for (int j = 0 ; j < width ; j++)
	{
	  if (*(marks_board+ j + (width*i)) == 0)
	    {
	      struct coord pos;
	      pos.y = i;
	      pos.x = j;
	      struct symbol current_symbol;
	      
	      current_symbol.coordinates =  g_array_new(0,1,sizeof(struct coord));
	      
	      
	      if (create_symbol (img,pos,marks_board ,current_symbol) == 1)
		{
		  determine_symbol_borders(&current_symbol);
		  determine_symbol_area(&current_symbol);
		  symbols = g_array_append_val(symbols,current_symbol);
		}
	      else
		{
		  g_array_free(current_symbol.coordinates, TRUE);
		}

	    }
	}
    }
  free (marks_board);
}

