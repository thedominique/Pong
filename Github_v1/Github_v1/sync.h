#pragma once
#ifndef sync_h
#define sync_h
#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>
//#include "constants.h"

long ticks_per_sec;
long tick_t0;

long next_net_tick;
long net_tick_interval;


void init_Tick();
#endif // !sync_h

