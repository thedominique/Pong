#include "event_handler.h"

int processEvents(SDL_Window *window, Paddle *mypaddle, double dt){
	SDL_Event event;
	int done = 0;

	//DETECT KEYS (ESC, CLOSE(X))
	while (SDL_PollEvent(&event)){
		switch (event.type){
		case SDL_WINDOWEVENT_CLOSE:{
			if (window){
				SDL_DestroyWindow(window);
				window = NULL;
				done = 1;
			}
		}
		break;
		case SDL_KEYDOWN:{
			switch (event.key.keysym.sym){
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
	//FLAGGOR TILL SERVER, COLLISION HANTERAS AV SERVERN
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_W]){
		mypaddle->k.w = 1;
	}
	if (state[SDL_SCANCODE_S]){
		mypaddle->k.s = 1;
	}
	if (state[SDL_SCANCODE_D]){
		mypaddle->k.d = 1;
	}
	if (state[SDL_SCANCODE_A]){
		mypaddle->k.a = 1;
	}
	if (state[SDL_SCANCODE_UP]){
		mypaddle->k.up = 1;
	}
	if (state[SDL_SCANCODE_DOWN]){
		mypaddle->k.down = 1;
	}

	return done;
}

void reset_keys(Paddle *mypaddle){
	mypaddle->k.w = 0;
	mypaddle->k.s = 0;
	mypaddle->k.a = 0;
	mypaddle->k.d = 0;
	mypaddle->k.up = 0;
	mypaddle->k.down = 0;
}
