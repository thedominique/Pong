#include "init_server_values.h"

void init_server_values(GameState *gamestate)
{
	gamestate->gameStarted = 0;
	gamestate->playerCounter = 0;
	gamestate->gameover = 0;
	gamestate->players[0].quit = 0;
	gamestate->players[0].playAgain = 0;
	gamestate->players[1].quit = 0;
	gamestate->players[1].playAgain = 0;
	gamestate->players[2].quit = 0;
	gamestate->players[2].playAgain = 0;

	gamestate->players[0].gameStarted = 0;
	gamestate->players[1].gameStarted = 0;
	gamestate->players[2].gameStarted = 0;

	gamestate->players[0].x = 30;
	gamestate->players[0].y = WINDOW_HEIGHT / 2;
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

	gamestate->players[2].x = 50;
	gamestate->players[2].y = 50;
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


	gamestate->ball.collision = 0;
}
