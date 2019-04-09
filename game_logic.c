#include "game_logic.h"
/*
void apply_game_logic(game_state_t* game_state){
    if (!game_state->is_server) {
        double* pad_y = game_state->is_player1 ? &game_state->pad1_y : &game_state->pad2_y;
        if (game_state->move_up != game_state->move_dn) {
           // *pad_y += (game_state->move_up? -PAD_SPEED_PER_SEC: PAD_SPEED_PER_SEC) * dt;
            *pad_y = MIN(1 - PAD_HEIGHT, *pad_y);
            *pad_y = MAX(0, *pad_y);
        }
    }
}

*/
