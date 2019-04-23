#include "rendering.h"

void doRender(SDL_Renderer *renderer, GameState *gamestate, Paddle *mypaddle, SDL_Texture *board, SDL_Texture *text){
	int i = 0;
	//set the drawing color to blue
	SDL_SetRenderDrawColor(renderer, 0, 0, 30, 255);

	//Clear the screen (to blue)
	SDL_RenderClear(renderer);

	//render TEXT
	SDL_SetRenderDrawColor(renderer, 248, 255, 11, 255);
	SDL_Rect textRect = { WINDOW_WIDTH / 2 - 150,0,300,50 };
	SDL_RenderCopy(renderer, text, NULL, &textRect);

	SDL_Rect boardRect = { WINDOW_WIDTH / 2 - WINDOW_WIDTH / 4 ,WINDOW_HEIGHT / 2 - WINDOW_HEIGHT / 4,WINDOW_WIDTH / 2,WINDOW_HEIGHT / 2  };
	SDL_RenderCopy(renderer, board, NULL, &boardRect);

	for (i = 0; i < 4; i++){
			//SET COLOR
			SDL_SetRenderDrawColor(renderer, gamestate->players[i].redShade, gamestate->players[i].greenShade, gamestate->players[i].blueShade, 255);
			//CREATE RECT
			SDL_Rect player = { gamestate->players[i].x, gamestate->players[i].y, gamestate->players[i].w, gamestate->players[i].h };
			//SHOW RECT
			SDL_RenderFillRect(renderer, &player);
		}
	//set the drawing color to white
	SDL_SetRenderDrawColor(renderer, gamestate->ball.redShade, gamestate->ball.greenShade, gamestate->ball.blueShade, 255);

	SDL_Rect ball = { gamestate->ball.x, gamestate->ball.y, gamestate->ball.w, gamestate->ball.h };
	SDL_RenderFillRect(renderer, &ball);
	SDL_RenderPresent(renderer);
}
