
#include "mainmenu.h"
#include <SDL.h>  
#include <SDL_image.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL_mixer.h>

int menu(SDL_Renderer *renderer)
{
	Mix_Music *music = NULL;	//Låt som används
	SDL_Event event;

	IMG_Init(IMG_INIT_JPG);
	Mix_Init(MIX_INIT_MP3);
	//Create Window

	SDL_Surface * image = IMG_Load("mainmenu2.jpg");
	SDL_Surface * image2 = IMG_Load("howto.jpg");
	SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, image);
	SDL_Texture * texture2 = SDL_CreateTextureFromSurface(renderer, image2);
	//Add Image
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	//Create invisible buttons over play and exit, adds image
	SDL_Rect playbox = { 290, 100, 470, 180 };
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);		//Test to see where rectangle is
	//SDL_RenderFillRect(renderer, &playbox);			//Test to see rectangle
	SDL_Rect howtobox = { 350, 345, 350, 100 };
	//SDL_RenderFillRect(renderer, &howtobox);
	SDL_Rect quitbox = { 320, 500, 200, 100 };
	//SDL_RenderFillRect(renderer, &quitbox);
	SDL_Rect backbox = { 470, 450, 270, 150 };
	//SDL_RenderFillRect(renderer, &backbox);
	SDL_RenderPresent(renderer);
	
	music = Mix_LoadWAV("MenuMus.wav");
	if (music == NULL)
	{
		printf("Music failed to load %s", SDL_GetError());
	}
	int channel = Mix_PlayChannel(-1, music, 0);
	
	int quit = 0;
	//Event handling, close window and button press
	while (!quit)
	{
		SDL_Event evt;
		while (SDL_PollEvent(&evt))
		{
			// quit on close, window close, or 'escape' key hit
			if (evt.type == SDL_QUIT ||
				(evt.type == SDL_WINDOWEVENT && evt.window.event == SDL_WINDOWEVENT_CLOSE) ||
				(evt.type == SDL_KEYDOWN && evt.key.keysym.sym == SDLK_ESCAPE)) 
			{
				quit = 1;
			}
			if (evt.type == SDL_MOUSEBUTTONDOWN &&
				evt.button.x >= playbox.x &&
				evt.button.x <= (playbox.x + playbox.w) &&
				evt.button.y >= playbox.y &&
				evt.button.y <= (playbox.h + playbox.y))
			{

				return 1;
			}
			else if (evt.type == SDL_MOUSEBUTTONDOWN &&
				evt.button.x >= howtobox.x &&
				evt.button.x <= (howtobox.x + howtobox.w) &&
				evt.button.y >= howtobox.y &&
				evt.button.y <= (howtobox.h + howtobox.y))
			{
				int waitresponse = 1;
				SDL_RenderCopy(renderer, texture2, NULL, NULL);
				SDL_RenderPresent(renderer);
				while (waitresponse)
				{
					while (SDL_PollEvent(&evt))
					{
						if (evt.type == SDL_MOUSEBUTTONDOWN &&
							evt.button.x >= backbox.x &&
							evt.button.x <= (backbox.x + backbox.w) &&
							evt.button.y >= backbox.y &&
							evt.button.y <= (backbox.h + backbox.y))
						{
							SDL_RenderCopy(renderer, texture, NULL, NULL);
							SDL_RenderPresent(renderer);
							waitresponse = 0;
						}
					}
				}
			}
			else if (evt.type == SDL_MOUSEBUTTONDOWN &&
				evt.button.x >= quitbox.x &&
				evt.button.x <= (quitbox.x + quitbox.w) &&
				evt.button.y >= quitbox.y &&
				evt.button.y <= (quitbox.h + quitbox.y))
			{
				IMG_Quit();
				SDL_Quit();
				return 2;
			}
		}
	}
	//Mix_FreeMusic(music);
	Mix_CloseAudio();
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(image);
	SDL_DestroyRenderer(renderer);

	IMG_Quit();
	SDL_Quit();
	return 0;
}