#ifndef objects_h
#define objects_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL_net.h"
#include "SDL.h"
#include <stdbool.h>


static const double BALL_SIZE = 20;
static const double WINDOW_WIDTH = 1280;
static const double WINDOW_HEIGHT = 720;
static const double BALL_SPEED = 0.002;
static const double PADDLE_WIDTH = 20;
static const double PADDLE_HEIGHT = 100;
static const double PADDLE_SPEED = 3;

/*
typedef struct
{
	int x, y, h, w, xVel, yVel;
} Paddle;
*/

typedef struct
{
	double x, y, h, w;
	int redShade, greenShade, blueShade;
	int lives;
} Paddle;



typedef struct
{
	double x, y, h, w, xVel, yVel;
	int redShade, greenShade, blueShade;
} Ball;



typedef struct
{
	Ball ball;
	Paddle players[3];
	int playerCounter;

} GameState;

typedef struct
{
	Ball b1;
	Paddle p1;
	Paddle p2;
} Receive;

#endif // !objects_h

