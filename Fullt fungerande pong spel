

#include "SDL.h"
#include <stdio.h>
#include <SDL_net.h>
#define BALL_SPEED 2
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 750
#define X_BALL_VEL 3
#define Y_BALL_VEL 3
#define PADDLE_WIDTH 40
#define PADDLE_HEIGHT 200

typedef struct
{
	int x, y, h, w,xVel,yVel;
} Paddle;




int processEvents(SDL_Window *window, Paddle *left_paddle, Paddle *right_paddle, Paddle *ball)
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

	//DETECT KEYS (UP, DOWN, LEFT, RIGHT)
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_W] && left_paddle->y>0)
	{
		left_paddle->y -= 20;
	}
	if (state[SDL_SCANCODE_S] && left_paddle->y < WINDOW_HEIGHT- left_paddle->h)
	{
		left_paddle->y += 20;
	}
	if (state[SDL_SCANCODE_UP] && right_paddle->y>0)
	{
		right_paddle->y -= 20;
	}
	if (state[SDL_SCANCODE_DOWN] && right_paddle->y < WINDOW_HEIGHT - right_paddle->h)
	{
		right_paddle->y += 20;
	}

	//collusion with window
	if (ball->y >= WINDOW_HEIGHT - ball->h)
	{
		ball->yVel = -ball->yVel;
	}
	if (ball->y <= 0)
	{
		ball->yVel = -ball->yVel;
	}
	if (ball->x >= WINDOW_WIDTH - ball->h)
	{
		ball->xVel = -ball->xVel;
	}
	if (ball->x <= 0)
	{
		ball->xVel = -ball->xVel;
	}


	//collision with padddelz
	if (collision_check(ball, left_paddle))
	{
		ball->xVel = -ball->xVel;
	}
	if (collision_check(ball, right_paddle))
	{
		ball->xVel = -ball->xVel;
	}

	//update position
	ball->x = ball->x + ball->xVel;
	ball->y = (ball->y) + (ball->yVel);



	return done;
}

int collision_check(Paddle *ball, Paddle *paddle)
{
	if (ball->y >= paddle->y + paddle->h)
	{
		return 0;
	}
	if (ball->x >= paddle->x + paddle->w)
	{
		return 0;
	}
	if (ball->y + ball->h <= paddle->y)
	{
		return 0;
	}
	if (ball->x + ball->w <= paddle->x)
	{
		return 0;
	}
	return 1;

		
}

void doRender(SDL_Renderer *renderer, Paddle *left_paddle, Paddle *right_paddle, Paddle *ball)
{
	//set the drawing color to blue
	SDL_SetRenderDrawColor(renderer, 0, 0, 40, 255);

	//Clear the screen (to blue)
	SDL_RenderClear(renderer);

	//set the drawing color to white
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	SDL_Rect rect = { left_paddle->x,left_paddle->y, PADDLE_WIDTH , PADDLE_HEIGHT };
	SDL_RenderFillRect(renderer, &rect);

	//We are done drawing, "present" or show to the screen what we've drawn

	SDL_Rect rect2 = { right_paddle->x, right_paddle->y, PADDLE_WIDTH , PADDLE_HEIGHT };
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

	Paddle left_paddle, right_paddle,ball;
	left_paddle.x = 0;
	left_paddle.y = WINDOW_HEIGHT/2 -PADDLE_HEIGHT/2;

	left_paddle.w = 40;
	left_paddle.h = 200;

	right_paddle.x = WINDOW_WIDTH - PADDLE_WIDTH;
	right_paddle.y = WINDOW_HEIGHT / 2 - PADDLE_HEIGHT / 2;;

	right_paddle.w = 50;
	right_paddle.h = 200;

	ball.x = WINDOW_WIDTH / 2;
	ball.y = WINDOW_HEIGHT / 2;
	ball.h = 20;
	ball.w = 20;
	ball.xVel = 7;
	ball.yVel = 7;
	

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
		done = processEvents(window, &left_paddle, &right_paddle,&ball);

		//Render display
		doRender(renderer, &left_paddle, &right_paddle,&ball);

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
