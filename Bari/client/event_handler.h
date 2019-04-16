#pragma once
#ifndef event_handler_h
#define event_handler_h
#include "objects.h"



int processEvents(SDL_Window *window, Paddle *mypaddle)
{
	SDL_Event event;
	int done = 0;

	//DETECT KEYS (ESC, CLOSE(X))
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
	if (state[SDL_SCANCODE_W] && mypaddle->y > 0)
	{
		mypaddle->y -= 1;
	}
	if (state[SDL_SCANCODE_S] && mypaddle->y < WINDOW_HEIGHT - mypaddle->h)
	{
		mypaddle->y += 1;
	}
	if (state[SDL_SCANCODE_D] && mypaddle->x < WINDOW_WIDTH - mypaddle->w)
	{
		mypaddle->x += 1;
	}
	if (state[SDL_SCANCODE_A] && mypaddle->x > 0)
	{
		mypaddle->x -= 1;
	}


	return done;
}







#endif // !event_handler_h