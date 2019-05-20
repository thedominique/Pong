#include "player_handler.h"

void get_client_values(UDPpacket *packet_receive, IPaddress client_ipaddress[], GameState *gamestate)
{
	int i = 0;
	int check = 0;
	for (i = 0; i < 3; i++)
	{
		if (packet_receive->address.host == client_ipaddress[i].host
			&& packet_receive->address.port == client_ipaddress[i].port)
		{
			Paddle* receive = (Paddle*)packet_receive->data;

			gamestate->keys[i].d = receive->k.d;
			gamestate->keys[i].a = receive->k.a;
			gamestate->keys[i].w = receive->k.w;
			gamestate->keys[i].s = receive->k.s;
			gamestate->keys[i].up = receive->k.up;
			gamestate->keys[i].down = receive->k.down;
			gamestate->players[i].playAgain = receive->playAgain;
			gamestate->players[i].quit = receive->quit;
			gamestate->players[i].gameStarted = receive->gameStarted;
		}
	}
}
/*
int check_player_ready(GameState *gamestate)
{
	int check = 0;
	if (( gamestate->players[0].playAgain == 1 || gamestate->players[0].quit == 1) && 
		(gamestate->players[1].playAgain == 1 || gamestate->players[1].quit == 1))
	{
		check = 1;
		printf("check: %d", check);
	}
	
	return check;
		
}
*/

int check_players_started(GameState *gamestate)
{
	if ((gamestate->players[0].gameStarted || gamestate->players[0].quit) &&
		(gamestate->players[1].gameStarted || gamestate->players[1].quit) &&
		(gamestate->players[2].gameStarted || gamestate->players[2].quit))
	{
		gamestate->gameStarted = 1;
		return 1;
	}
	else
	{
		return 0;
	}
}





int check_player_ready(GameState *gamestate)
{
	int check = 0;

	for (int i = 0; i < 3 ; i++)
	{
		if (gamestate->players[i].playAgain || gamestate->players[i].quit)
		{
			check ++;
		}
		else
		{
			check--;
		}
	}
	
	if (check == 3)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void reset_game(GameState *gamestate)
{
	/*
	for (int i = 0; i < 3; i++)
	{
		if (gamestate->players[i].quit == 1 && gamestate->gameover)
		{
			gamestate->playerCounter--;
			gamestate->players[i].quit = 0;
		}
	}
	*/
	
	if (check_player_ready(gamestate))
	{

		gamestate->gameover = 0;
		gamestate->players[0].playAgain = 0;
		gamestate->players[1].playAgain = 0;
		gamestate->players[2].playAgain = 0;



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
	}
}
