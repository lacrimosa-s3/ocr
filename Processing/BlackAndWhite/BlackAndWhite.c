#include <err.h>
#include <math.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL_tools.h"


int grayscale(char path[], char dest[]) // used in BlackandWhite
{
    SDL_Surface* image_surface;
    init_sdl();
    image_surface = load_image(path);
    int height = image_surface->h;
    int width = image_surface->w;
    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            Uint32 pixel = get_pixel(image_surface, i, j);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
            Uint8 average = 0.3 * r + 0.59 * g + 0.11 * b;
            r = average;
            g = average;
            b = average;
            Uint32 new_pixel = SDL_MapRGB(image_surface->format, r, g, b);
            put_pixel(image_surface,i,j,new_pixel);
        }
    }

    SDL_SaveBMP(image_surface, dest); 
    return 0;
}


void makehisto(SDL_Surface *image, int rows,int cols, unsigned long *histo) // used in otsu
{
    for(int i =0; i<rows; i++)
    {

        for(int j =0; i<cols; i++)
        {

            Uint32 pixel = get_pixel(image, i, j);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, image->format, &r, &g, &b);
            histo[r] +=1;
        }
    }
} 

int otsu(SDL_Surface *image, int rows, int cols) // used in Blackandwhite
{
    unsigned long *histo = calloc(256,sizeof(unsigned long));
    makehisto( image, rows,cols,histo);

    unsigned long nb_pxl= (unsigned long)rows*cols;
    unsigned long top = 256;
    unsigned long wF = 0;
    unsigned long wB = 0;
    unsigned long max =0;
    unsigned long sumB=0 ;
    unsigned long sum1=0 ;
    int level= 0;

    for(unsigned long j = 1; j<top;j++)
    {
        sum1 = j*histo[j];
    }
    for(int i =0; i<256; i++)
    {

        wB+=histo[i];
        if(wB==0){
            continue;
        }

        wF = nb_pxl -wB;
        if(wF==0){
            break;
        }

        sumB +=  (i*histo[i]);

        unsigned long mB = sumB/wB;
        unsigned long mF = (sum1-sumB)/wF;

        float val = wB*wF*(mB-mF)*(mB-mF);
            if(val>=max)
            {
                level= i;
                max = val;
            }
    }
    return level;
}


int BlackandWhite(char source[], char dest[])
{
    SDL_Surface* image_surface;
    init_sdl();
    image_surface = load_image(source);

    grayscale(source, dest);

    image_surface = load_image(dest);

    int height = image_surface->h;
    int width = image_surface->w;

    int tresh = otsu(image_surface, width, height);

    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            Uint32 pixel = get_pixel(image_surface, i, j);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
            if (r >= tresh)
            {
                r = 255;
                g = 255;
                b = 255;
            }
            else
            {
                r = 0;
                g = 0;
                b = 0;
            }

            Uint32 new_pixel = SDL_MapRGB(image_surface->format, r, g, b);
            put_pixel(image_surface,i,j,new_pixel);    
        }
    }
    SDL_SaveBMP(image_surface, dest);
    SDL_FreeSurface(image_surface);
    return 0;
}


int main()
{
    BlackandWhite("image.jpeg","result.jpeg");
    return 0;
}
