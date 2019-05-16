#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include "SDL_net.h"
#include <stdbool.h>
#include "server_clientqueue.h"
#include "collision.h"
#include "objects.h"
#include "player_handler.h"

int main(int argc, char **argv)
{
	int i = 0;
	SDL_Init(SDL_INIT_EVERYTHING);
	SDLNet_Init(SDL_INIT_EVERYTHING);
	UDPsocket server_socket;
	IPaddress server_ipaddress;
	IPaddress client_ipaddress[3] = { 1,1,1 };
	UDPpacket *packet_send;
	UDPpacket *packet_receive;

	GameState gamestate;

	gamestate.playerCounter = 0;
	gamestate.ball.collision = 0;

	gamestate.players[0].x = 30;
	gamestate.players[0].y = WINDOW_HEIGHT / 2;
	gamestate.players[0].w = PADDLE_WIDTH;
	gamestate.players[0].h = PADDLE_HEIGHT;
	gamestate.players[0].redShade = 75;
	gamestate.players[0].greenShade = 0;
	gamestate.players[0].blueShade = 130;
	gamestate.players[0].lives = 3;
	gamestate.players[0].gameOver = SDL_FALSE;

	gamestate.players[1].x = WINDOW_WIDTH - PADDLE_WIDTH;
	gamestate.players[1].y = WINDOW_HEIGHT / 2;
	gamestate.players[1].w = PADDLE_WIDTH;
	gamestate.players[1].h = PADDLE_HEIGHT;
	gamestate.players[1].redShade = 255;
	gamestate.players[1].greenShade = 0;
	gamestate.players[1].blueShade = 0;
	gamestate.players[1].lives = 3;
	gamestate.players[1].gameOver = SDL_FALSE;

	gamestate.players[2].x = 50;
	gamestate.players[2].y = 50;
	gamestate.players[2].w = PADDLE_WIDTH;
	gamestate.players[2].h = PADDLE_HEIGHT;
	gamestate.players[2].redShade = 248;
	gamestate.players[2].greenShade = 255;
	gamestate.players[2].blueShade = 11;
	gamestate.players[2].lives = 3;
	gamestate.players[2].gameOver = SDL_FALSE;

	gamestate.runBall = SDL_TRUE;

	gamestate.ball.x = WINDOW_WIDTH / 2;
	gamestate.ball.y = WINDOW_HEIGHT / 2;
	gamestate.ball.h = BALL_SIZE;
	gamestate.ball.w = BALL_SIZE;
	gamestate.ball.xVel = BALL_SPEED;
	gamestate.ball.yVel = BALL_SPEED;
	gamestate.ball.redShade = 75;
	gamestate.ball.greenShade = 0;
	gamestate.ball.blueShade = 130;

	for (i = 0; i < 3; i++)
	{
		gamestate.keys[i].up = 0;
		gamestate.keys[i].down = 0;
	}

	float NET_TICK_RATE = 60;

	init_array_struct(client_ipaddress);
	printf("%d %d\n", client_ipaddress[3].host, client_ipaddress[3].port);

	/* Open a socket on random port */
	server_socket = SDLNet_UDP_Open(1234);

	/* Resolve server name  */
	SDLNet_ResolveHost(&server_ipaddress, NULL, 1234);

	packet_send = SDLNet_AllocPacket(512);
	packet_receive = SDLNet_AllocPacket(512);
	//packet_send->len = strlen((char *)packet_send->data) + 1;
	//packet_receive->len = strlen((char *)packet_receive->data) + 1;

	long ticks_per_sec = SDL_GetPerformanceFrequency();
	long tick_t0 = SDL_GetPerformanceCounter();
	long next_net_tick = tick_t0;
	long net_tick_interval = (1 / NET_TICK_RATE) * ticks_per_sec;

	while (1)
	{
		long tick_t1 = SDL_GetPerformanceCounter();
		double dt = (tick_t1 - tick_t0) / (double)ticks_per_sec;


		if (SDLNet_UDP_Recv(server_socket, packet_receive))
		{
			//Paddle* receive = (Paddle*)packet_receive->data;
			//printf("RE: %lf \n", receive->x);
			check_ipaddress(packet_receive, client_ipaddress, &gamestate);
			identify_player(packet_receive, client_ipaddress, &gamestate);
		}
		//printf("TRIED TO SEND ball x: %lf\n", gamestate.ball.x);
		//printf("Lives player 0: %d\n", gamestate.players[0].lives);
		//printf("gamestate player[0].x: %lf\n", gamestate.players[0].x);

		//HANTERAR COLLISION OCH KEY PRESSES
		key_handler(&gamestate);
		
		if (gamestate.runBall == SDL_TRUE)
		{
			detect_collision(&gamestate);
		}
		if (tick_t1 >= next_net_tick)
		{
			for (i = 0; i < 3; i++)
			{
				//key_handler(&gamestate);
				if (client_ipaddress[i].port != 0)
				{
					packet_send->data = (void*)&gamestate;
					packet_send->channel = -1;
					packet_send->len = sizeof(gamestate);
					packet_send->maxlen = packet_send->len + 20;
					packet_send->address.host = client_ipaddress[i].host;
					packet_send->address.port = client_ipaddress[i].port;

					if (SDLNet_UDP_Send(server_socket, -1, packet_send) == 0)
					{
						printf("failed to send from server/n");
					}
					gamestate.ball.collision = 0;
				}
			}
			next_net_tick += net_tick_interval; 
		}
	}
	SDLNet_FreePacket(packet_send);
	SDLNet_FreePacket(packet_receive);
	SDLNet_Quit();
}
