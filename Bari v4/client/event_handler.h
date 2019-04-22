#pragma once
#ifndef event_handler_h
#define event_handler_h
#include "objects.h"


int collision_check(Paddle *mypaddle)
{
	if (WINDOW_HEIGHT / 2 - WINDOW_HEIGHT / 4 >= mypaddle->y + mypaddle->h)
	{
		return 0;
	}
	if (WINDOW_WIDTH / 2 - WINDOW_WIDTH / 4 >=  mypaddle->x )
	{
		return 0;
	}
	if (WINDOW_HEIGHT / 2 - WINDOW_HEIGHT / 4 + WINDOW_HEIGHT / 2 <= mypaddle->y)
	{
		return 0;
	}
	if (WINDOW_WIDTH / 2 - WINDOW_WIDTH / 4 + WINDOW_WIDTH / 2 <= mypaddle->x + mypaddle->w)
	{
		return 0;
	}
	return 1;


}

int processEvents(SDL_Window *window, Paddle *mypaddle, double dt)
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



	// SKICKA FLAGGOR SOM SÄGER GÅ FRAMÅT, OCH SEDAN RESET DOM OM DET HÄR SKA VARA I SERVER

	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_W] && mypaddle->y >= 0)
	{
		mypaddle->y -= PADDLE_SPEED;

		if (collision_check(mypaddle))
		{
			mypaddle->y += PADDLE_SPEED;
		}
	}
	if (state[SDL_SCANCODE_S] && mypaddle->y < WINDOW_HEIGHT - mypaddle->h && !collision_check(mypaddle))
	{
		mypaddle->y += PADDLE_SPEED;

		if (collision_check(mypaddle))
		{
			mypaddle->y -= PADDLE_SPEED;
		}
	}
	if (state[SDL_SCANCODE_D] && mypaddle->x < WINDOW_WIDTH - mypaddle->w && !collision_check(mypaddle))
	{
		mypaddle->x += PADDLE_SPEED;

		if (collision_check(mypaddle))
		{
			mypaddle->x -= PADDLE_SPEED;
		}
	}
	if (state[SDL_SCANCODE_A] && mypaddle->x > 0 && !collision_check(mypaddle))
	{
		mypaddle->x -= PADDLE_SPEED;

		if (collision_check(mypaddle))
		{
			mypaddle->x += PADDLE_SPEED;
		}
	}


	return done;
}







#endif // !event_handler_h