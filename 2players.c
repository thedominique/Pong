

#include "SDL.h"
#include <stdio.h>
#include <SDL_net.h>
#define BALL_SPEED 1
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 750


typedef struct
{
	int x, y, h, w;
} Man;




int processEvents(SDL_Window *window, Man *man, Man *man2, Man *ball)
{
	SDL_Event event;
	int done = 0;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_WINDOWEVENT_CLOSE:
		{
			if (window)
			{
				SDL_DestroyWindow(window);
				window = NULL;
				done = 1;
			}
		}
		break;
		case SDL_KEYDOWN:
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				done = 1;
				break;
			}
		}
		break;
		case SDL_QUIT:
			//quit out of the game
			done = 1;
			break;
		}
	}

	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_UP] && man->y>0)
	{
		man->y -= 20;
	}
	if (state[SDL_SCANCODE_DOWN] && man->y < WINDOW_HEIGHT- man->h)
	{
		man->y += 20;
	}
	if (state[SDL_SCANCODE_W] && man2->y>0)
	{
		man2->y -= 20;
	}
	if (state[SDL_SCANCODE_S] && man2->y < WINDOW_HEIGHT - man2->h)
	{
		man2->y += 20;
	}

	ball->x = BALL_SPEED+ball->x;
	ball->y = BALL_SPEED+ball->y;

	if (ball->y < WINDOW_HEIGHT - ball->h)
	{

	}
	return done;
}

void doRender(SDL_Renderer *renderer, Man *man, Man *man2, Man *ball)
{
	//set the drawing color to blue
	SDL_SetRenderDrawColor(renderer, 0, 0, 40, 255);

	//Clear the screen (to blue)
	SDL_RenderClear(renderer);

	//set the drawing color to white
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	SDL_Rect rect = { man->x,man->y,40, 200 };
	SDL_RenderFillRect(renderer, &rect);

	//We are done drawing, "present" or show to the screen what we've drawn

	SDL_Rect rect2 = { man2->x, man2->y,40,200 };
	SDL_RenderFillRect(renderer, &rect2);

	SDL_Rect rect3 = {ball->x,ball->y,20,20 };
	SDL_RenderFillRect(renderer, &rect3);


	SDL_RenderPresent(renderer);


}


int main(int argc, char *argv[])
{
	SDL_Window *window;                    // Declare a window
	SDL_Renderer *renderer;                // Declare a renderer

	SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2

	Man man, man2,ball;
	man.x = 10;
	man.y = 50;

	man.w = 50;
	man.h = 200;

	man2.x = 740;
	man2.y = 50;

	man2.w = 50;
	man2.h = 200;

	ball.x = WINDOW_WIDTH / 2;
	ball.y = WINDOW_HEIGHT / 2;
	ball.h = 20;
	ball.w = 20;
	

	//Create an application window with the following settings:
	window = SDL_CreateWindow("Game Window",                     // window title
		SDL_WINDOWPOS_UNDEFINED,           // initial x position
		SDL_WINDOWPOS_UNDEFINED,           // initial y position
		WINDOW_WIDTH,                      // width, in pixels
		WINDOW_HEIGHT,                     // height, in pixels
		0                                  // flags
	);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);



	// The window is open: enter program loop (see SDL_PollEvent)
	int done = 0;

	//Event loop
	while (!done)
	{
		//Check for events
		done = processEvents(window, &man, &man2,&ball);

		//Render display
		doRender(renderer, &man, &man2,&ball);

		//don't burn up the CPU
		SDL_Delay(1000/60);
	}


	// Close and destroy the window
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	// Clean up
	SDL_Quit();
	return 0;
}
