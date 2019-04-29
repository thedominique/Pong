#pragma once
#ifndef objects_h
#define objects_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL_net.h"
#include "SDL.h"


static const double BALL_SIZE = 20;
static const double WINDOW_WIDTH = 1280;
static const double WINDOW_HEIGHT = 720;
static const double BALL_SPEED = 0.00009;
static const double PADDLE_WIDTH = 20;
static const double PADDLE_HEIGHT = 100;
static const double PADDLE_SPEED = 2;



typedef struct
{
	int w, s, a, d, up, down;
} Keys;

typedef struct
{
	int player1Lives, player2Lives, player3Lives;
	char purple[7];
	char red[4];
	char yellow[7];
	char lives[6];
	char msg[32];
} OldLives;

typedef struct
{
	double x, y, h, w;
	int redShade, greenShade, blueShade;
	int lives;
	Keys k;
} Paddle;




typedef struct
{
	double x, y, h, w, xVel, yVel;
	int redShade, greenShade, blueShade;
	int collision;
} Ball;



typedef struct
{
	Ball ball;
	Paddle players[3];
	int collision;
} GameState;

typedef struct
{
	Ball b1;
	Paddle p[3];
} Receive;

#endif // !objects_h
