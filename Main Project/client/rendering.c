#include "rendering.h"

void doRender(SDL_Renderer *renderer, GameState *gamestate, Paddle *mypaddle, SDL_Texture *board, SDL_Texture *text, SDL_Texture *player1Texture, SDL_Texture *player2Texture, SDL_Texture *player3Texture) {
	int i = 0;
	//set the drawing color to blue
	SDL_SetRenderDrawColor(renderer, 0, 0, 30, 255);

	//Clear the screen (to blue)
	SDL_RenderClear(renderer);

	//render TEXT
	SDL_SetRenderDrawColor(renderer, 248, 255, 11, 255);
	SDL_Rect textRect = { WINDOW_WIDTH / 2 - 150,0,300,50 };
	SDL_RenderCopy(renderer, text, NULL, &textRect);

	SDL_Rect boardRect = { WINDOW_WIDTH / 2 - WINDOW_WIDTH / 4 ,WINDOW_HEIGHT / 2 - WINDOW_HEIGHT / 4,WINDOW_WIDTH / 2,WINDOW_HEIGHT / 2 };
	SDL_RenderCopy(renderer, board, NULL, &boardRect);

	SDL_Rect player1 = { gamestate->players[0].x, gamestate->players[0].y, gamestate->players[0].w, gamestate->players[0].h };
	SDL_RenderCopy(renderer, player1Texture, NULL, &player1);
	
	SDL_Rect player2 = { gamestate->players[1].x, gamestate->players[1].y, gamestate->players[1].w, gamestate->players[1].h };
	SDL_RenderCopy(renderer, player2Texture, NULL, &player2);
	
	SDL_Rect player3 = { gamestate->players[2].x, gamestate->players[2].y, gamestate->players[2].w, gamestate->players[2].h };
	SDL_RenderCopy(renderer, player3Texture, NULL, &player3);

	//set the drawing color to white
	SDL_SetRenderDrawColor(renderer, gamestate->ball.redShade, gamestate->ball.greenShade, gamestate->ball.blueShade, 255);
	SDL_Rect ball = { gamestate->ball.x, gamestate->ball.y, gamestate->ball.w, gamestate->ball.h };
	SDL_RenderFillRect(renderer, &ball);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(renderer, &ball);

	SDL_RenderPresent(renderer);
}
