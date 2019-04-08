#include <stdio.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#define MENUITEMS 3

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

char *files[]={
    "isthis.jpg", "JPG",
    NULL,
    "lena.xbm", "XBM",
    NULL
};


void poll_event(SDL_Renderer* renderer){
    for(;;){
        SDL_Event e;
        if(SDL_PollEvent(&e)){
            switch(e.type ){
                case SDL_QUIT:
                    return;
                default:
                    break;
            }
        }
    }
}


int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
    
    SDL_Surface* image;
    int i;
    
    SDL_Window* window = SDL_CreateWindow("Hello, SDL2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    
    
    
    for(i=0; files[i]; i+=2)
    {
        /* load the image */
        image=IMG_Load(files[i]);
        if(!image)
        {
            /* image failed to load */
            printf("IMG_Load: %s\n", IMG_GetError());
            printf("%s is not supported, or the image data is corrupted\n", files[i+1]);
            files[i+1]=NULL;
            continue;
        }
        /* report success */
        printf("%s is supported\n", files[i+1]);
        /* free the loaded image */
        SDL_FreeSurface(image);
    }
    
    
    IMG_Quit();
    SDL_Quit();
    return 0;
}
