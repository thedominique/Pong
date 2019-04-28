#include "collision.h"

void next_color_after_death(GameState *gamestate)
{
	int i;

	for (i = 0; i < 4; i++)
	{
		if (gamestate->players[i].lives != 0)
		{
			gamestate->ball.redShade = gamestate->players[i].redShade;
			gamestate->ball.greenShade = gamestate->players[i].greenShade;
			gamestate->ball.blueShade = gamestate->players[i].blueShade;
			return;
		}
	}

}


void kill_player(GameState *gamestate)
{
	int i = which_player(gamestate);
	gamestate->players[i].x = 0;
	gamestate->players[i].y = 0;
	gamestate->players[i].w = 0;
	gamestate->players[i].h = 0;
	next_color_after_death(gamestate);
}


void collision_with_paddles(GameState *gamestate, int i) 
{
	gamestate->ball.x = gamestate->ball.x + gamestate->ball.xVel;

	if (collision_check(gamestate, i) && equal_color(gamestate, i)) 
	{
		gamestate->ball.x = gamestate->ball.x - gamestate->ball.xVel + negativ_or_positiv(gamestate->ball.xVel);
		gamestate->ball.xVel = -gamestate->ball.xVel;
		give_next_color(gamestate, i);
		increase_ball_speed(gamestate);
	}

	gamestate->ball.y = gamestate->ball.y + gamestate->ball.yVel;

	if (collision_check(gamestate, i) && equal_color(gamestate, i)) {
		gamestate->ball.y = gamestate->ball.y - gamestate->ball.yVel + negativ_or_positiv(gamestate->ball.yVel);
		gamestate->ball.yVel = -gamestate->ball.yVel;
		give_next_color(gamestate, i);
		increase_ball_speed(gamestate);

	}
}

int collision_check(GameState *gamestate, int i) {
	if (gamestate->ball.y >= gamestate->players[i].y + gamestate->players[i].h) 
	{
		return 0;
	}
	if (gamestate->ball.x >= gamestate->players[i].x + gamestate->players[i].w)
	{
		return 0;
	}
	if (gamestate->ball.y + gamestate->ball.h <= gamestate->players[i].y) 
	{
		return 0;
	}
	if (gamestate->ball.x + gamestate->ball.w <= gamestate->players[i].x)
	{
		return 0;
	}
	return 1;
}

void detect_collision(GameState *gamestate)
{
	int i = 0;
	// BALL collision with WINDOW
	if (gamestate->ball.y >= WINDOW_HEIGHT / 2 + WINDOW_HEIGHT / 4 - gamestate->ball.h)
	{
		gamestate->ball.yVel = -gamestate->ball.yVel;
	}
	if (gamestate->ball.y <= WINDOW_HEIGHT / 4)
	{
		gamestate->ball.yVel = -gamestate->ball.yVel;
	}
	if (gamestate->ball.x >= WINDOW_WIDTH / 2 + WINDOW_WIDTH / 4 - gamestate->ball.h)
	{
		resetState(gamestate);
		if (gamestate->players[which_player(gamestate)].lives != 0)
		{
			gamestate->players[which_player(gamestate)].lives--;
			if (gamestate->players[which_player(gamestate)].lives == 0)
			{
				kill_player(gamestate);
			}
		}
	}
	if (gamestate->ball.x <= WINDOW_WIDTH / 4)
	{
		resetState(gamestate);
		if (gamestate->players[which_player(gamestate)].lives != 0)
		{
			gamestate->players[which_player(gamestate)].lives--;
			if (gamestate->players[which_player(gamestate)].lives == 0)
			{
				kill_player(gamestate);
			}
		}
	}


	// just nu checkar den bara 3 spelare
	for (i = 0; i < 3; i++)
	{
		collision_with_paddles(gamestate, i);
	}


}


void increase_ball_speed(GameState *gamestate)
{
	if (gamestate->ball.xVel> 0)
	{
		gamestate->ball.xVel += BALL_SPEED;
		if (gamestate->ball.yVel > 0)
		{
			gamestate->ball.yVel += BALL_SPEED;
		}
		else 
		{
			gamestate->ball.yVel -= BALL_SPEED;
		}
		return;
	}
	if (gamestate->ball.xVel < 0)
	{
		gamestate->ball.xVel -= BALL_SPEED;
		if (gamestate->ball.yVel > 0)
		{
			gamestate->ball.yVel += BALL_SPEED;
		}
		else
		{
			gamestate->ball.yVel -= BALL_SPEED;
		}
		return;
	}

}

int negativ_or_positiv(double velocity) {
	if (velocity > 0) {
		return -20;
	}
	if (velocity < 0) {
		return 20;
	}
	return -100; // ad hoc ugly af solution plz change
}

int which_player(GameState *gamestate) {
	for (int i = 0; i < 3; i++) {
		if (gamestate->players[i].redShade == gamestate->ball.redShade &&
			gamestate->players[i].greenShade == gamestate->ball.greenShade &&
			gamestate->players[i].blueShade == gamestate->ball.blueShade) {
			return i;
		}
	}
	return 4; // ad hoc ugly af solution - Domp
}


bool equal_color(GameState *gamestate, int i) {
	if (gamestate->players[i].redShade == gamestate->ball.redShade &&
		gamestate->players[i].greenShade == gamestate->ball.greenShade &&
		gamestate->players[i].blueShade == gamestate->ball.blueShade) {
		return true;
	}
	else {
		return false;
	}
}

void give_next_color(GameState *gamestate, int i) {
	if (i == gamestate->playerCounter - 1 && gamestate->players[0].lives !=0) 
	{
		gamestate->ball.redShade = 75;
		gamestate->ball.greenShade = 0;
		gamestate->ball.blueShade = 130;
		return;
	}
	else if(gamestate->players[i + 1].lives !=0)
	{
		gamestate->ball.redShade = gamestate->players[i + 1].redShade;
		gamestate->ball.greenShade = gamestate->players[i + 1].greenShade;
		gamestate->ball.blueShade = gamestate->players[i + 1].blueShade;

	}
	else if (gamestate->players[i - 1].lives != 0)
	{
		gamestate->ball.redShade = gamestate->players[i - 1].redShade;
		gamestate->ball.greenShade = gamestate->players[i - 1].greenShade;
		gamestate->ball.blueShade = gamestate->players[i - 1].blueShade;
	}
}

void resetState(GameState* gamestate) {

	gamestate->ball.x = WINDOW_WIDTH / 2;
	gamestate->ball.y = WINDOW_HEIGHT / 2;
	gamestate->ball.h = BALL_SIZE;
	gamestate->ball.w = BALL_SIZE;
	gamestate->ball.xVel = BALL_SPEED;
	gamestate->ball.yVel = BALL_SPEED;

}