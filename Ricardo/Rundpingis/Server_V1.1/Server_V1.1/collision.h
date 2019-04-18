#pragma once
#ifndef collision_h
#define collision_h
#include "SDL_net.h"
#include <SDL.h>
#include "objects.h"

void detect_collision(GameState *gamestate)
{
	int i = 0;
	// BALL collision with WINDOW
	//if (gamestate->ball.y >= WINDOW_HEIGHT - gamestate->ball.h)
	if (gamestate->ball.y >= (540) - gamestate->ball.h)
	{
		gamestate->ball.yVel = -gamestate->ball.yVel;
	}
	if (gamestate->ball.y <= 180)
	//if (gamestate->ball.y <= WINDOW_HEIGHT)
	{
		gamestate->ball.yVel = -gamestate->ball.yVel;
	}
	//if (gamestate->ball.x >= WINDOW_WIDTH - gamestate->ball.h)
	if (gamestate->ball.x >= 980 + PADDLE_WIDTH - gamestate->ball.h)
	{
		//gamestate->ball.xVel = -gamestate->ball.xVel;
		printf("IM in!\n");
		resetState(gamestate);
	}
	//if (gamestate->ball.x <= 0)
	if (gamestate->ball.x <= TABLE_X - PADDLE_WIDTH)
	{
		printf("im in!\n");
		//gamestate->ball.xVel = -gamestate->ball.xVel;
		resetState(gamestate);
	}

	// just nu checkar den bara 2 spelare
	for (i = 0; i < 2; i++)
	{
		if (collision_check(gamestate, i))
		{
			printf("player %d\n", i +1);
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

	printf("Hej\n");
	return 1;
}

int resetState(GameState* gamestate)
{
	/*gamestate->players[0].x = 0;
	gamestate->players[0].y = WINDOW_HEIGHT / 2;
	gamestate->players[0].w = PADDLE_WIDTH;
	gamestate->players[0].h = PADDLE_HEIGHT;

	gamestate->players[1].x = WINDOW_WIDTH - PADDLE_WIDTH;
	gamestate->players[1].y = WINDOW_HEIGHT / 2;
	gamestate->players[1].w = PADDLE_WIDTH;
	gamestate->players[1].h = PADDLE_HEIGHT;*/

	gamestate->ball.x = WINDOW_WIDTH / 2;
	gamestate->ball.y = WINDOW_HEIGHT / 2;
	gamestate->ball.h = BALL_SIZE;
	gamestate->ball.w = BALL_SIZE;
	gamestate->ball.xVel = BALL_SPEED;
	gamestate->ball.yVel = BALL_SPEED;
	gamestate->ball.reset = 1;
}


#endif // !collision_h