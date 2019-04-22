#pragma once
#ifndef display_text_h
#define display_text_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL_net.h"
#include "SDL.h"
#include "objects.h"
#include "SDL_ttf.h"



int compare_lives(GameState *gamestate, OldLives *oldlives)
{
	if ((gamestate->players[0].lives == oldlives->player1Lives &&
		gamestate->players[1].lives == oldlives->player2Lives &&
		gamestate->players[2].lives == oldlives->player3Lives))
	{
		return 0;
	}
	else
	{
		return 1;
	}
}




SDL_Texture *update_text(GameState *gamestate, SDL_Renderer *renderer, OldLives *oldLives)
{


	TTF_Font *font = TTF_OpenFont("Arial.ttf", 20);
	if (!font)
	{
		printf("TTF open error");
	}
	//reset msg =0;
	sprintf(oldLives->msg, "%s: %s %d %s %d %s %d", oldLives->lives, oldLives->purple , gamestate->players[0].lives, oldLives->red, gamestate->players[1].lives, oldLives->yellow, gamestate->players[2].lives);


	SDL_Color color = { 255,255,255,255 };
	SDL_Surface *textSurface = TTF_RenderText_Solid(font, oldLives->msg, color);
	SDL_Texture *text = SDL_CreateTextureFromSurface(renderer, textSurface);

	SDL_FreeSurface(textSurface);

	oldLives->player1Lives = gamestate->players[0].lives;
	oldLives->player2Lives = gamestate->players[1].lives;
	oldLives->player3Lives = gamestate->players[2].lives;


	return text;
}




SDL_Texture* init_text(GameState *gamestate, SDL_Renderer *renderer)
{

	if (TTF_Init() < 0)
	{
		printf("TTF error");
	}

	TTF_Font *font = TTF_OpenFont("Arial.ttf", 20);
	if (!font)
	{
		printf("TTF open error");
	}

	char str[32];
	char str2[7] = "purple";
	char str3[4] = "red";
	char str4[7] = "yellow";
	char str5[6] = "lives";
	sprintf(str, "%s: %s %d %s %d %s %d", str5, str2, gamestate->players[0].lives, str3, gamestate->players[1].lives, str4, gamestate->players[2].lives);




	SDL_Color color = { 255,255,255,255 };
	SDL_Surface *textSurface = TTF_RenderText_Solid(font, str, color);
	SDL_Texture *text = SDL_CreateTextureFromSurface(renderer, textSurface);

	SDL_FreeSurface(textSurface);

	return text;
}








#endif // !display_text_h
