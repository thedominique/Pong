#ifndef server_event_handler_h
#define server_event_handler_h
#include "objects.h"


int paddle_board_collision(GameState *gamestate, int i);
int paddle_paddle_collision(GameState *gamestate, int i, int j);
int collision_loop(GameState *gamestate, int i);
void key_handler(GameState *gamestate);





#endif // !server_event_handler_h

