#ifndef game_state_h
#define game_state_h

#include <stdbool.h>
#include <stdio.h>
#include "SDL_net.h"
#include <SDL2/SDL.h>

typedef struct game_state {
    double pad1_y;
    double pad2_y;
    double ball_x;
    double ball_y;
    double ball_dv_x, ball_dv_y;
    
    IPaddress client_addr[2];
    
    bool is_player1;
    
    bool move_up;
    bool move_dn;
    
    long ticks_per_sec;
    long tick_t0;
    
    long next_net_tick;
    long net_tick_interval;
    
    bool quit;
    
} game_state_t;

void init_game_state(game_state_t* game_state);


#endif /* game_state_h */
