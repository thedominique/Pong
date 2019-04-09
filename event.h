#ifndef event_h
#define event_h

#include "game_state.h"
#include <stdio.h>
#include "SDL_net.h"
#include <SDL2/SDL.h>

void poll_events(game_state_t* game_state);

#endif /* event_h */
