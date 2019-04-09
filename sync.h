#ifndef sync_h
#define sync_h

#include <stdbool.h>
#include <stdio.h>
#include "SDL_net.h"
#include <SDL2/SDL.h>

long ticks_per_sec;
long tick_t0;

long next_net_tick;
long net_tick_interval;

#endif /* sync_h */
