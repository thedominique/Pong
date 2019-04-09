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
    
    IPaddress server_addr;
    int local_port = 0;
    bool is_server = false;
    
    //init_render();
    
    return 0;
}
