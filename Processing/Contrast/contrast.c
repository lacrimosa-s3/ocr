#include <err.h>
#include <math.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL_tools.h"


void ContrastEnhancement(char source[], char dest[])
{
    SDL_Surface* image_surface;

    init_sdl();

    image_surface = load_image(source);
    
    // Contrast Enhancement process 
    int height = image_surface->h;
    int width = image_surface->w;

    int size = height*width;
    int curr = 0;
    int hist[256] = {0};
    int *mat = malloc(size * sizeof(int));


    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            Uint32 pixel = get_pixel(image_surface, j, i);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
            float average = 0.3 * r + 0.59 * g + 0.11 * b;
            int index = average;
            hist[index] += 1;
            mat[i * width + j] = average;
            pixel = SDL_MapRGB(image_surface->format, average, average, average);
            put_pixel(image_surface,j,i,pixel);
        }
    }

    for(int k = 0; k != 256; k++)
    {

        curr += hist[k];
        hist[k] = round(curr * 255 / size);
    }



    for (int k = 0; k < 256; k++)
    {
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                if (mat[i* width + j] == k)
                {
                    mat[i* width + j] = hist[k];

                }
            }
        }
    }


    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            Uint32 pixel = SDL_MapRGB(image_surface->format, mat[i * width + j], mat[i * width + j], mat[i * width + j]);
            put_pixel(image_surface,j,i,pixel);
        }
    }

    SDL_SaveBMP(image_surface, dest);
    SDL_FreeSurface(image_surface);
}


int main()
{
    ContrastEnhancement("image.jpeg", "result.jpeg");
    return 0;
}
