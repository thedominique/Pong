//#include "game_events.h"
//#include <SDL.h>
//#include "display_text.h"
#include "rendering.h"

void request_Next_Round(Paddle* mypaddle) {
	mypaddle->runBall = SDL_FALSE;
}

void isPlayerOut(GameState* gamestate, Paddle* mypaddle) {
	for (int i = 0; i < 3; i++)
	{
		if (gamestate->players[i].lives == 0 && gamestate->players[i].gameOver == SDL_FALSE)
		{
			gamestate->players[i].gameOver = SDL_TRUE;
			gamestate->runBall = SDL_FALSE;
			printf("Player %d is out\n", i);
			request_Next_Round(mypaddle);
		}
	}
}
