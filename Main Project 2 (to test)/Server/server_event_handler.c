#include "server_event_handler.h"



int paddle_board_collision(GameState *gamestate, int i)
{
	if (WINDOW_HEIGHT / 2 - WINDOW_HEIGHT / 4 - 1 >= gamestate->players[i].y + gamestate->players[i].h)
	{
		return 0;
	}

	if (gamestate->players[i].w == PADDLE_WIDTH)
	{

		if (WINDOW_WIDTH / 2 - WINDOW_WIDTH / 4 >= gamestate->players[i].x)
		{
			return 0;
		}

	}
	else
	{
		if (WINDOW_WIDTH / 2 - WINDOW_WIDTH / 4 >= gamestate->players[i].x + PADDLE_HEIGHT - PADDLE_WIDTH)
		{
			return 0;
		}
	}


	if (WINDOW_HEIGHT / 2 - WINDOW_HEIGHT / 4 + WINDOW_HEIGHT / 2 + 1 <= gamestate->players[i].y)
	{
		return 0;
	}
	if (WINDOW_WIDTH / 2 - WINDOW_WIDTH / 4 + WINDOW_WIDTH / 2 <= gamestate->players[i].x + PADDLE_WIDTH)
	{
		return 0;
	}
	return 1;
}


int paddle_paddle_collision(GameState *gamestate, int i, int j)
{


	if (gamestate->players[i].y >= gamestate->players[j].y + gamestate->players[j].h)
	{

		return 0;
	}
	if (gamestate->players[i].x >= gamestate->players[j].x + gamestate->players[j].w)
	{

		return 0;
	}
	if (gamestate->players[i].y + gamestate->players[i].h <= gamestate->players[j].y)
	{

		return 0;
	}
	if (gamestate->players[i].x + gamestate->players[i].w <= gamestate->players[j].x)
	{

		return 0;
	}

	return 1;

}

int collision_loop(GameState *gamestate, int i)
{

	int j = 0;
	int result = 0;

	for (j = 0; j < 3; j++)
	{
		if (i != j)
		{
			if (paddle_paddle_collision(gamestate, i, j) == 1)
			{
				result = 1;
			}
		}
	}

	return result;
}


void key_handler(GameState *gamestate)
{
	int i = 0;
	double temp = 0;

	for (i = 0; i < 3; i++)
	{

		if (gamestate->keys[i].w && gamestate->players[i].y >= 0)
		{
			gamestate->players[i].y -= PADDLE_SPEED;
			gamestate->keys[i].w = 0;


			if (paddle_board_collision(gamestate, i) || collision_loop(gamestate, i))
			{
				gamestate->players[i].y += PADDLE_SPEED;
			}
		}

		if (gamestate->keys[i].s && gamestate->players[i].y < WINDOW_HEIGHT - gamestate->players[i].h)
		{
			gamestate->players[i].y += PADDLE_SPEED;
			gamestate->keys[i].s = 0;


			if (paddle_board_collision(gamestate, i) || collision_loop(gamestate, i))
			{
				gamestate->players[i].y -= PADDLE_SPEED;
			}
		}

		if (gamestate->keys[i].d && gamestate->players[i].x < WINDOW_WIDTH - gamestate->players[i].w)
		{
			gamestate->players[i].x += PADDLE_SPEED;
			gamestate->keys[i].d = 0;


			if (paddle_board_collision(gamestate, i) || collision_loop(gamestate, i))
			{
				gamestate->players[i].x -= PADDLE_SPEED;
			}
		}

		if (gamestate->keys[i].a && gamestate->players[i].x > 0)
		{
			gamestate->players[i].x -= PADDLE_SPEED;
			gamestate->keys[i].a = 0;


			if (paddle_board_collision(gamestate, i) || collision_loop(gamestate, i))
			{
				gamestate->players[i].x += PADDLE_SPEED;
			}
		}
		if (gamestate->keys[i].up && gamestate->players[i].lives != 0)
		{
			gamestate->players[i].w = PADDLE_WIDTH;
			gamestate->players[i].h = PADDLE_HEIGHT;
			gamestate->keys[i].up = 0;
			if (paddle_board_collision(gamestate, i) || collision_loop(gamestate, i) || gamestate->players[i].y > WINDOW_HEIGHT - PADDLE_HEIGHT)
			{
				gamestate->players[i].w = PADDLE_HEIGHT;
				gamestate->players[i].h = PADDLE_WIDTH;
			}
		}
		if (gamestate->keys[i].down && gamestate->players[i].lives != 0)
		{
			gamestate->players[i].w = PADDLE_HEIGHT;
			gamestate->players[i].h = PADDLE_WIDTH;
			gamestate->keys[i].down = 0;
			if (paddle_board_collision(gamestate, i) || collision_loop(gamestate, i) || gamestate->players[i].x + gamestate->players[i].w > WINDOW_WIDTH)
			{
				gamestate->players[i].w = PADDLE_WIDTH;
				gamestate->players[i].h = PADDLE_HEIGHT;
			}
		}
	}
}