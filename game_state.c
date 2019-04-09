#include "game_state.h"

#include "constants.h"

void init_game_state(game_state_t* game_state){
    game_state->pad1_y = 0;
    game_state->pad2_y = 0;
    game_state->ball_x = BALL_INIT_X0;
    game_state->ball_y = BALL_INIT_Y0;
    game_state->ball_dv_x = BALL_SPEED_PER_SEC;
    game_state->ball_dv_y = BALL_SPEED_PER_SEC;
    
    memset(game_state->client_addr, 0, sizeof(game_state->client_addr));
    
    game_state->is_player1 = true;
    game_state->is_server = false;
    
    game_state->move_up = false;
    game_state->move_dn = false;
    
    game_state->ticks_per_sec = SDL_GetPerformanceFrequency();
    game_state->tick_t0 = SDL_GetPerformanceCounter();
    
    game_state->next_net_tick = game_state->tick_t0;
    game_state->net_tick_interval = (1 / NET_TICK_RATE) * game_state->ticks_per_sec;
    
    game_state->quit = false;
}
