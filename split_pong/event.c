#include "event.h"

void poll_events(game_state_t* game_state) {
    SDL_Event e;
    
    if (SDL_PollEvent(&e) == 0) {
        return;
    }
    switch (e.type) {
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            switch (e.key.keysym.sym) {
                case SDLK_UP:
                    game_state->move_up = (e.type == SDL_KEYDOWN);
                    break;
                case SDLK_DOWN:
                    game_state->move_dn = (e.type == SDL_KEYDOWN);
                    break;
                default:
                    break;
            }
            break;
        case SDL_QUIT:
            game_state->quit = true;
            break;
        default:
            break;
    }
}
