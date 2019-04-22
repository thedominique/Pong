#ifndef collision_h
#define collision_h

#include <stdio.h>
#include <stdbool.h>
#include "objects.h"

void collision_with_paddles(GameState *gamestate, int i);
int collision_check(GameState *gamestate, int i);
void detect_collision(GameState *gamestate);

int negativ_or_positiv(double velocity);


int which_player(GameState *gamestate);

bool equal_color(GameState *gamestate, int i);
void give_next_color(GameState *gamestate, int i);

void resetState(GameState* gamestate);



// gamestate->playes[(which_player(gamestate)].lives--;
/*
 int which_player(GameState *gamestate)
 {
 int i;
 
 for(i=0;i<3;i++)
 
 }
 
 */







#endif // !collision_h
