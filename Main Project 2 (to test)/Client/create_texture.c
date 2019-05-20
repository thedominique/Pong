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


SDL_Texture *create_player_texture(SDL_Renderer *renderer, char image_name[])
{
	SDL_Texture *playerTexture;
	SDL_Surface *playerSurface = NULL;
	playerSurface = IMG_Load(image_name);
	if (playerSurface == NULL)
	{
		printf("Could not find image! \n");
		SDL_Quit();
		return 1;
	}
	playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);
	SDL_FreeSurface(playerSurface);
	return playerTexture;
}

SDL_Texture *init_background(SDL_Renderer *renderer)
{
	SDL_Texture *backgroundTexture;
	SDL_Surface *backgroundSurface= NULL;
	backgroundSurface = IMG_Load("background.jpg");
	if (backgroundSurface == NULL)
	{
		printf("Could not find background image!\n");
		SDL_Quit();
		return 1;
	}
	backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
	SDL_FreeSurface(backgroundSurface);
	return backgroundTexture;
}