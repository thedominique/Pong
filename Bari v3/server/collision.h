#pragma once
#ifndef collision_h
#define collision_h
#include "SDL_net.h"
#include <SDL.h>
#include "objects.h"

void collision_with_paddles(GameState *gamestate, int i);
int negativ_or_positiv(double velocity);
void resetState(GameState* gamestate);
bool equal_color(GameState *gamestate, int i);
void give_next_color(GameState *gamestate, int i);


// gamestate->playes[(which_player(gamestate)].lives--;
/*
	int which_player(GameState *gamestate)
	{
		int i;
		
		for(i=0;i<3;i++)
		


	}
		
*/

int which_player(GameState *gamestate)
{
	int i = 0;

	for (i = 0; i < 3; i++)
	{
		if (gamestate->players[i].redShade == gamestate->ball.redShade &&
			gamestate->players[i].greenShade == gamestate->ball.greenShade &&
			gamestate->players[i].blueShade == gamestate->ball.blueShade)
		{
			return i;
		}
	}
}


void detect_collision(GameState *gamestate)
{
	int i = 0;
	// BALL collision with WINDOW
	if (gamestate->ball.y >= WINDOW_HEIGHT / 2 + WINDOW_HEIGHT / 4 - gamestate->ball.h)
	{
		gamestate->ball.yVel = -gamestate->ball.yVel;
	}
	if (gamestate->ball.y <= WINDOW_HEIGHT / 4)
	{
		gamestate->ball.yVel = -gamestate->ball.yVel;
	}
	if (gamestate->ball.x >= WINDOW_WIDTH / 2 + WINDOW_WIDTH / 4 - gamestate->ball.h)
	{
		resetState(gamestate);
		gamestate->players[which_player(gamestate)].lives--;
		//gamestate->ball.xVel = -gamestate->ball.xVel;
	}
	if (gamestate->ball.x <=  WINDOW_WIDTH / 4)
	{
		resetState(gamestate);
		gamestate->players[which_player(gamestate)].lives--;
		//gamestate->ball.xVel = -gamestate->ball.xVel;
	}

	/*
	for (i = 0; i < 2; i++)
	{
		if (collision_check(gamestate, i))
		{
			gamestate->ball.x = gamestate->ball.x - gamestate->ball.xVel+10;
			gamestate->ball.y = gamestate->ball.y - gamestate->ball.yVel+10;
			gamestate->ball.xVel = -gamestate->ball.xVel;
			gamestate->ball.yVel = -gamestate->ball.yVel;
		}
	}
	*/
	//update BALL position 
	//gamestate->ball.x = gamestate->ball.x + gamestate->ball.xVel;
	//gamestate->ball.y = gamestate->ball.y + gamestate->ball.yVel;

	// just nu checkar den bara 3 spelare
	for (i = 0; i < 3; i++)
	{
		collision_with_paddles(gamestate, i);
	}


}

int negativ_or_positiv(double velocity)
{
	if (velocity > 0)
	{
		return -20;
	}
	if (velocity < 0)
	{
		return 20;
	}
}


void collision_with_paddles(GameState *gamestate, int i)
{
	gamestate->ball.x = gamestate->ball.x + gamestate->ball.xVel;
	
	if (collision_check(gamestate,i) && equal_color(gamestate,i))
	{
		gamestate->ball.x = gamestate->ball.x - gamestate->ball.xVel + negativ_or_positiv(gamestate->ball.xVel);
		gamestate->ball.xVel = -gamestate->ball.xVel;
		give_next_color(gamestate, i);
	}

	gamestate->ball.y = gamestate->ball.y + gamestate->ball.yVel;

	if (collision_check(gamestate,i) && equal_color(gamestate, i))
	{
		gamestate->ball.y = gamestate->ball.y - gamestate->ball.yVel + negativ_or_positiv(gamestate->ball.yVel);
		gamestate->ball.yVel = -gamestate->ball.yVel;
		give_next_color(gamestate, i);
	}

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

void resetState(GameState* gamestate)
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

}


bool equal_color(GameState *gamestate, int i)
{

	if (gamestate->players[i].redShade == gamestate->ball.redShade &&
		gamestate->players[i].greenShade == gamestate->ball.greenShade &&
		gamestate->players[i].blueShade == gamestate->ball.blueShade)
	{
		return true;
	}
	else
	{
		return false;
	}
}



void give_next_color(GameState *gamestate, int i)
{
	if (i == gamestate->playerCounter -1)
	{
		gamestate->ball.redShade = 75;
		gamestate->ball.greenShade = 0;
		gamestate->ball.blueShade = 130;
		return;
	}
	else
	{
		gamestate->ball.redShade = gamestate->players[i + 1].redShade;
		gamestate->ball.greenShade = gamestate->players[i + 1].greenShade;
		gamestate->ball.blueShade = gamestate->players[i + 1].blueShade;

	}


}

#endif // !collision_h
