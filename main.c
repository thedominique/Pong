//
//  main.c
//  hoeFilePong
//
//  Created by Dominique Persson on 2019-04-07.
//  Copyright © 2019 Dominique Persson. All rights reserved.
//


#include <stdio.h>
#include "SDL_net.h"
#include <SDL2/SDL.h>
#include <string.h>
#include <stdbool.h>

#include "sync.h"
#include "rendering.h"


int main(int argc, const char * argv[]) {
    
    SDL_Init(SDL_INIT_EVERYTHING);
    
    SDL_Window* window = SDL_CreateWindow(
                                          "Ultra pong Yee Yee",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SCREEN_WIDTH,
                                          SCREEN_HEIGHT,
                                          SDL_WINDOW_OPENGL);
    
    if (window == NULL) {
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Could not create renderer: %s\n", SDL_GetError());
        return 1;
    }
    windowRendering(renderer);
    
    return 0;
}
