#include "init_master.h"
#include "objects.h"

void initPlayers(GameState* gamestate, OldLives* oldlives, Paddle *mypaddle) {
	printf("Check");
	gamestate->players[0].x = WINDOW_WIDTH / 2;
	gamestate->players[0].y = 30;
	gamestate->players[0].w = PADDLE_WIDTH;
	gamestate->players[0].h = PADDLE_HEIGHT;
	gamestate->players[0].redShade = 75;
	gamestate->players[0].greenShade = 0;
	gamestate->players[0].blueShade = 130;
	gamestate->players[0].lives = 3;


	gamestate->players[1].x = WINDOW_WIDTH - PADDLE_WIDTH;
	gamestate->players[1].y = WINDOW_HEIGHT / 2;
	gamestate->players[1].w = PADDLE_WIDTH;
	gamestate->players[1].h = PADDLE_HEIGHT;
	gamestate->players[1].redShade = 255;
	gamestate->players[1].greenShade = 0;
	gamestate->players[1].blueShade = 0;
	gamestate->players[1].lives = 3;

	gamestate->players[2].x = 0;
	gamestate->players[2].y = 0;
	gamestate->players[2].w = PADDLE_WIDTH;
	gamestate->players[2].h = PADDLE_HEIGHT;
	gamestate->players[2].redShade = 248;
	gamestate->players[2].greenShade = 255;
	gamestate->players[2].blueShade = 11;
	gamestate->players[2].lives = 3;


	gamestate->ball.x = WINDOW_WIDTH / 2;
	gamestate->ball.y = WINDOW_HEIGHT / 2;
	gamestate->ball.h = BALL_SIZE;
	gamestate->ball.w = BALL_SIZE;
	gamestate->ball.xVel = BALL_SPEED;
	gamestate->ball.yVel = BALL_SPEED;
	gamestate->ball.redShade = 75;
	gamestate->ball.greenShade = 0;
	gamestate->ball.blueShade = 130;

	gamestate->gameover = 0;
	gamestate->gameStarted = 0;
	

	oldlives->player1Lives = gamestate->players[0].lives = 3;
	oldlives->player2Lives = gamestate->players[1].lives = 3;
	oldlives->player3Lives = gamestate->players[2].lives = 3;

	strcpy(oldlives->purple, "purple");
	strcpy(oldlives->red, "red");
	strcpy(oldlives->yellow, "yellow");
	strcpy(oldlives->lives, "Lives");

	mypaddle->x = 30;
	mypaddle->y = 30;
	mypaddle->k.w = 0;
	mypaddle->k.w = 0;
	mypaddle->k.w = 0;
	mypaddle->k.w = 0;
	mypaddle->k.up = 0;
	mypaddle->k.down = 0;
	mypaddle->playAgain = 0;
	mypaddle->quit = 0;
	mypaddle->gameStarted = 0;

	gamestate->ball.collision = 0;
}