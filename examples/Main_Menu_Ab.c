#include <SDL.h>  
#include <SDL_image.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char ** argv)
{
	bool pressed = false;
	bool pressed1 = false;
	bool quit = false;
	SDL_Event event;

	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_JPG);
	//Create Window
	SDL_Window * window = SDL_CreateWindow("SDL2 Displaying Image", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, 0);
	//Create render, surface and texture
	SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
	SDL_Surface * image = IMG_Load("mainmenu.jpg");
	SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, image);
	//Create invisible buttons over play and exit, adds image
	SDL_Rect dstrect = { 5, 5, 320, 240 };
	SDL_RenderCopy(renderer, texture, NULL, &dstrect);
	SDL_RenderCopy(renderer, texture, NULL, NULL);

	SDL_Rect box = { 290, 100, 470, 180 };
	//SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);		//Test to see where rectangle is
	//SDL_RenderFillRect(renderer, &box);
	SDL_Rect box2 = { 270, 365, 300, 100 };
	//SDL_RenderFillRect(renderer, &box2);
	SDL_RenderPresent(renderer);

	//Event handling, close window and button press
	while (!quit)
	{
		pressed = false;
		pressed1 = false;
		SDL_Event evt;
		while (SDL_PollEvent(&evt))
		{
			// quit on close, window close, or 'escape' key hit
			if (evt.type == SDL_QUIT ||
				(evt.type == SDL_WINDOWEVENT && evt.window.event == SDL_WINDOWEVENT_CLOSE) ||
				(evt.type == SDL_KEYDOWN && evt.key.keysym.sym == SDLK_ESCAPE)) {
				quit = 1;
			}
			if (evt.type == SDL_MOUSEBUTTONDOWN && 
				evt.button.x >= box.x && 
				evt.button.x <= (box.x + box.w) && 
				evt.button.y >= box.y && 
				evt.button.y <= (box.h + box.y))
			{
				
				pressed = true;
			}
			else if (evt.type == SDL_MOUSEBUTTONDOWN &&
				evt.button.x >= box2.x &&
				evt.button.x <= (box2.x + box2.w) &&
				evt.button.y >= box2.y &&
				evt.button.y <= (box2.h + box2.y))
			{
				pressed1 = true;
			}
			if (pressed == true)
			{
				printf("DU HAR TRYCKT PLAY\n");			// HÄR STARTAR SPEL SKIT
			}
			if (pressed1 == true)
			{
				printf("DU HAR TRYCKT QUIT\n");			//STÄNG NER SKIT
			}
		}
	}

	SDL_DestroyTexture(texture);
	SDL_FreeSurface(image);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	IMG_Quit();
	SDL_Quit();

	return 0;
}
