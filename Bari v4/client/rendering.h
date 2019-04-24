#ifndef rendering_h
#define rendering_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL_net.h"
#include "SDL.h"
#include "objects.h"

void doRender(SDL_Renderer *renderer, GameState *gamestate, Paddle *mypaddle, SDL_Texture *board, SDL_Texture *text);
#endif // !1
