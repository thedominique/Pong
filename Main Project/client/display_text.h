#ifndef display_text_h
#define display_text_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL_net.h"
#include "SDL.h"
#include "objects.h"
#include "SDL_ttf.h"


int compare_lives(GameState *gamestate, OldLives *oldlives);

SDL_Texture *update_text(GameState *gamestate, SDL_Renderer *renderer, OldLives *oldLives);

SDL_Texture* init_text(GameState *gamestate, SDL_Renderer *renderer);

#endif // !display_text_h