#pragma once
#ifndef rendering_h
#define rendering_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL_net.h"
#include "SDL.h"
#include "objects.h"

void doRender(SDL_Renderer *renderer, GameState *gamestate, Paddle *mypaddle) {

	//set the drawing color to blue
	SDL_SetRenderDrawColor(renderer, 0, 0, 30, 255);

	//Clear the screen (to blue)
	SDL_RenderClear(renderer);

	//draw table
	SDL_SetRenderDrawColor(renderer, 118, 88, 44, 255);
	SDL_Rect table = { TABLE_X, TABLE_Y, TABLE_WIDTH, TABLE_HEIGHT };
	SDL_RenderFillRect(renderer, &table);

	// border for right-side of table
	SDL_SetRenderDrawColor(renderer, 216, 163, 84, 255);
	SDL_Rect rightborder = { 960, TABLE_Y, PADDLE_WIDTH, TABLE_HEIGHT };
	SDL_RenderFillRect(renderer, &rightborder);

	// border for left-side of table
	SDL_SetRenderDrawColor(renderer, 216, 163, 84, 255);
	SDL_Rect leftborder = { 320, TABLE_Y, PADDLE_WIDTH, TABLE_HEIGHT };
	SDL_RenderFillRect(renderer, &leftborder);


	//draw net
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_Rect net = { NET_X - BALL_SIZE, NET_Y, 20, NET_HEIGHT };
	SDL_RenderFillRect(renderer, &net);


	//set the drawing color to purple
	SDL_SetRenderDrawColor(renderer, 75, 0, 130, 255);

	SDL_Rect player1 = { gamestate->players[0].x,gamestate->players[0].y, PADDLE_WIDTH , PADDLE_HEIGHT };
	SDL_RenderFillRect(renderer, &player1);

	//set the drawing color to green
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

	SDL_Rect player2 = { gamestate->players[1].x, gamestate->players[1].y, PADDLE_WIDTH, PADDLE_HEIGHT };
	SDL_RenderFillRect(renderer, &player2);


	//set the drawing color to white
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	SDL_Rect ball = { gamestate->ball.x, gamestate->ball.y, gamestate->ball.w, gamestate->ball.h };
	SDL_RenderFillRect(renderer, &ball);
	SDL_RenderPresent(renderer);
}

#endif // !1