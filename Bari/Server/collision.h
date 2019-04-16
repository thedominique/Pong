#pragma once
#ifndef collision_h
#define collision_h
#include "SDL_net.h"
#include <SDL.h>
#include "objects.h"

// make a .c file this is disgusting

void detect_collision(GameState *gamestate)
{
	int i = 0;
	// BALL collision with WINDOW
	if (gamestate->ball.y >= WINDOW_HEIGHT - gamestate->ball.h)
	{
		gamestate->ball.yVel = -gamestate->ball.yVel;
	}
	if (gamestate->ball.y <= 0)
	{
		gamestate->ball.yVel = -gamestate->ball.yVel;
	}
	if (gamestate->ball.x >= WINDOW_WIDTH - gamestate->ball.h)
	{
		gamestate->ball.xVel = -gamestate->ball.xVel;
	}
	if (gamestate->ball.x <= 0)
	{
		gamestate->ball.xVel = -gamestate->ball.xVel;
	}

	// just nu checkar den bara 2 spelare
	for (i = 0; i < 2; i++)
	{
		if (collision_check(gamestate, i))
		{
			gamestate->ball.xVel = -gamestate->ball.xVel;
			gamestate->ball.yVel = -gamestate->ball.yVel;
		}
	}

	//update BALL position 
	gamestate->ball.x = gamestate->ball.x + gamestate->ball.xVel;
	gamestate->ball.y = gamestate->ball.y + gamestate->ball.yVel;

}


int collision_check(GameState *gamestate, int i)
{
	if (gamestate->ball.y >= gamestate->players[i].y + gamestate->players[i].h)
	{
		return 0;
	}
	if (gamestate->ball.x >= gamestate->players[i].x + gamestate->players[i].w)
	{
		return 0;
	}
	if (gamestate->ball.y + gamestate->ball.h <= gamestate->players[i].y)
	{
		return 0;
	}
	if (gamestate->ball.x + gamestate->ball.w <= gamestate->players[i].x)
	{
			return 0;
	}

	return 1;
}



#endif // !collision_h
