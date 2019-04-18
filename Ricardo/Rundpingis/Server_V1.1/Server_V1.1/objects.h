#pragma once
#ifndef objects_h
#define objects_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL_net.h"
#include "SDL.h"

const double BALL_SIZE = 20;
const double WINDOW_WIDTH = 1280;
const double WINDOW_HEIGHT = 720;
const double BALL_SPEED = 0.0005;
const double PADDLE_WIDTH = 20;
const double PADDLE_HEIGHT = 100;
const double NET_TICK_RATE = 60;

const double TABLE_X = 1280 / 4;
double TABLE_Y = 720 / 4;
double TABLE_WIDTH = 1280 / 2;
double TABLE_HEIGHT = 720 / 2;
double NET_X = 1280 / 2;
double NET_Y = 720 / 4;
double NET_HEIGHT = 720 / 2;
/*
typedef struct
{
	int x, y, h, w, xVel, yVel;
} Paddle;
*/

typedef struct {
	double x, y, h, w;
} Paddle;

typedef struct {
	double x, y, h, w, xVel, yVel;
	int reset;
} Ball;

typedef struct {
	Ball ball;
	Paddle players[3];
} GameState;

typedef struct {
	Ball b1;
	Paddle p[3];
} Receive;

#endif // !objects_h