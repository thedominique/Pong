#pragma once
#ifndef render_h
#define render_h
#include <SDL.h>
#include "constants.h"
#include "game_state.h"

// fungerade inte tidigare. Den här löser problemet.
// äldre kod står under kommenterad.
SDL_Renderer* init_Window() {
	SDL_Window* window;
	SDL_Renderer* render;
	window = SDL_CreateWindow(
		"YEET",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_W,
		SCREEN_H,
		0);
	if (!window)
	{
		printf("WINDOW OOF: %s\n", SDL_GetError());
		exit(2);
	}
	render = SDL_CreateRenderer(
		window, -1, SDL_RENDERER_ACCELERATED);
	if (!render)
	{
		printf("RENDER OOF: %s\n", SDL_GetError());
		exit(2);
	}

	return render;
}


/*int init_Window(SDL_Window* window, SDL_Renderer* render) {
	window = SDL_CreateWindow(
		"YEET",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_W,
		SCREEN_H,
		0);
	if (!window)
	{
		printf("WINDOW OOF: %s\n", SDL_GetError());
		return 1;
	}
	render = SDL_CreateRenderer(
		window, -1, SDL_RENDERER_ACCELERATED);
	if (!render)
	{
		printf("RENDER OOF: %s\n", SDL_GetError());
		return 1;
	}
	return 0;
}*/

void windowRendering(SDL_Renderer* renderer, gamestate_t* gamestate, Players_t player[]) {
	//SDL_SetRenderDrawColor(renderer, 0, 0, 40, 0xFF);

	SDL_RenderClear(renderer);


	// Draw pad 1 (left pad).
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_Rect fillRect = { p1_posX0 + player[0].xPos , p1_posY0 + player[0].yPos, PAD_WIDTH, PAD_HEIGHT };
	SDL_RenderFillRect(renderer, &fillRect);

	// Draw pad 2.
	SDL_SetRenderDrawColor(renderer, 255, 0x00, 0x00, 255);
	SDL_Rect fillRect2 = { (p2_posX0 - PAD_WIDTH - 5) + player[1].xPos, (p2_posY0)+player[1].yPos, PAD_WIDTH, PAD_HEIGHT };
	SDL_RenderFillRect(renderer, &fillRect2);

	// Draw ball.
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Byter färg
	SDL_Rect fillRect3 = { gamestate->ball_xPos , gamestate->ball_yPos, BALL_WIDTH , BALL_HEIGHT };
	SDL_RenderFillRect(renderer, &fillRect3);

	SDL_RenderPresent(renderer);
}

#endif // !render_h

