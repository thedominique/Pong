#include "rendering.h"

void windowRendering(SDL_Renderer* renderer, game_state_t* game_state){
    SDL_RenderClear(renderer);
    
    SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
    
    // Draw pad 1.
    SDL_Rect fillRect = { PAD_PAD * SCREEN_WIDTH, game_state->pad1_y*SCREEN_HEIGHT, PAD_WIDTH * SCREEN_WIDTH, PAD_HEIGHT * SCREEN_HEIGHT };
    SDL_RenderFillRect( renderer, &fillRect );
    
    
    // Draw pad 2.
    SDL_Rect fillRect2 = { (1.0 - PAD_PAD - PAD_WIDTH) * SCREEN_WIDTH, game_state->pad2_y*SCREEN_HEIGHT, PAD_WIDTH * SCREEN_WIDTH, PAD_HEIGHT * SCREEN_HEIGHT };
    SDL_RenderFillRect( renderer, &fillRect2 );
    
    // Draw ball.
    SDL_Rect fillRect3 = {game_state->ball_x * SCREEN_WIDTH, game_state->ball_y*SCREEN_HEIGHT, BALL_WIDTH * SCREEN_WIDTH, BALL_HEIGHT * SCREEN_HEIGHT };
    SDL_RenderFillRect( renderer, &fillRect3 );

    SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xFF );
    SDL_RenderPresent(renderer);
}

int init_render(){
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
    return 0;
}
