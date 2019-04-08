#include "rendering.h"

#include "SDL_net.h"
#include <SDL2/SDL.h>


void windowRendering(SDL_Renderer* renderer){
    SDL_RenderClear(renderer);
    
    SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
    
    // Draw pad 1.
    SDL_Rect fillRect = { PAD_PAD * SCREEN_WIDTH, pad1_y*SCREEN_HEIGHT, PAD_WIDTH * SCREEN_WIDTH, PAD_HEIGHT * SCREEN_HEIGHT };
    SDL_RenderFillRect( renderer, &fillRect );
    
    // Draw pad 2.
    SDL_Rect fillRect2 = { (1.0 - PAD_PAD - PAD_WIDTH) * SCREEN_WIDTH, pad2_y*SCREEN_HEIGHT, PAD_WIDTH * SCREEN_WIDTH, PAD_HEIGHT * SCREEN_HEIGHT };
    SDL_RenderFillRect( renderer, &fillRect2 );
    
    // Draw ball.
    SDL_Rect fillRect3 = {ball_x * SCREEN_WIDTH, ball_y*SCREEN_HEIGHT, BALL_WIDTH * SCREEN_WIDTH, BALL_HEIGHT * SCREEN_HEIGHT };
    SDL_RenderFillRect( renderer, &fillRect3 );
    
    SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xFF );
    SDL_RenderPresent(renderer);
}
