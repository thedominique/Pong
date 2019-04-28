#include "create_texture.h"


SDL_Texture *init_board_texture(SDL_Renderer *renderer)
{


	SDL_Texture *board;
	SDL_Surface *boardSurface = NULL;
	boardSurface = IMG_Load("bana.jpg");
	if (boardSurface == NULL)
	{
		printf("Could not find image! \n");
		SDL_Quit();
		return 1;
	}
	board = SDL_CreateTextureFromSurface(renderer, boardSurface);
	SDL_FreeSurface(boardSurface);
	return board;

}


SDL_Texture *init_player1_texture(SDL_Renderer *renderer)
{
	SDL_Texture *player1Texture;
	SDL_Surface *playerSurface = NULL;
	playerSurface = IMG_Load("player1.jpg");
	if (playerSurface == NULL)
	{
		printf("Could not find image! \n");
		SDL_Quit();
		return 1;
	}
	player1Texture = SDL_CreateTextureFromSurface(renderer, playerSurface);
	SDL_FreeSurface(playerSurface);
	return player1Texture;
}


SDL_Texture *init_player2_texture(SDL_Renderer *renderer)
{
	SDL_Texture *player2Texture;
	SDL_Surface *playerSurface = NULL;
	playerSurface = IMG_Load("player2.jpg");
	if (playerSurface == NULL)
	{
		printf("Could not find image! \n");
		SDL_Quit();
		return 1;
	}
	player2Texture = SDL_CreateTextureFromSurface(renderer, playerSurface);
	SDL_FreeSurface(playerSurface);
	return player2Texture;
}

SDL_Texture *init_player3_texture(SDL_Renderer *renderer)
{
	SDL_Texture *player3Texture;
	SDL_Surface *playerSurface = NULL;
	playerSurface = IMG_Load("player3.jpg");
	if (playerSurface == NULL)
	{
		printf("Could not find image! \n");
		SDL_Quit();
		return 1;
	}
	player3Texture = SDL_CreateTextureFromSurface(renderer, playerSurface);
	SDL_FreeSurface(playerSurface);
	return player3Texture;
}