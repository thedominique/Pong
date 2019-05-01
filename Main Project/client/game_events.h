#ifndef game_events_h
#define game_events_h
#include <SDL.h> 
#include "objects.h"

void request_Next_Round(Paddle* mypaddle);

void isPlayerOut(GameState* gamestate, Paddle* mypaddle);

#endif // !game_events_h
