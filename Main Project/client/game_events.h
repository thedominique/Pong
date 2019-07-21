#ifndef game_events_h
#define game_events_h
#include "game_events.h"
#include <SDL.h>
#include "display_text.h"
#include "objects.h"

void request_Next_Round(Paddle* mypaddle);

void isPlayerOut(GameState* gamestate, Paddle* mypaddle);

#endif // !game_events_h
