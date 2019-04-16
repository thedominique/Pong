#pragma once
#ifndef objects_h
#define objects_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL_net.h"
#include "SDL.h"


const double BALL_SIZE = 20;
const double WINDOW_WIDTH = 1200;
const double WINDOW_HEIGHT = 750;
const double BALL_SPEED = 0.005;
const double PADDLE_WIDTH = 40;
const double PADDLE_HEIGHT = 200;


/*
typedef struct
{
	int x, y, h, w, xVel, yVel;
} Paddle;
*/

typedef struct
{
	double x, y, h, w;
} Paddle;



typedef struct
{
	double x, y, h, w, xVel, yVel;
} Ball;



typedef struct
{
	Ball ball;
	Paddle players[3];

} GameState;

typedef struct
{
	Ball b1;
	Paddle p[3];
} Receive;

#endif // !objects_h
