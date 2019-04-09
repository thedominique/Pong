#ifndef rendering_h
#define rendering_h

#include "constants.h"
#include "game_state.h"
#include "SDL_net.h"
#include <SDL2/SDL.h>
#include <stdio.h>

void windowRendering(SDL_Renderer* renderer, game_state_t* game_state);
int init_render();
#endif /* rendering_h */
