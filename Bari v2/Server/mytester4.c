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
	
	gamestate.players[0].x = 0;
	gamestate.players[0].y = WINDOW_HEIGHT / 2;
	gamestate.players[0].w = PADDLE_WIDTH;
	gamestate.players[0].h = PADDLE_HEIGHT;

	gamestate.players[1].x = WINDOW_WIDTH - PADDLE_WIDTH;
	gamestate.players[1].y = WINDOW_HEIGHT / 2;
	gamestate.players[1].w = PADDLE_WIDTH;
	gamestate.players[1].h = PADDLE_HEIGHT;

	gamestate.ball.x = WINDOW_WIDTH / 2;
	gamestate.ball.y = WINDOW_HEIGHT / 2;
	gamestate.ball.h = BALL_SIZE;
	gamestate.ball.w = BALL_SIZE;
	gamestate.ball.xVel = BALL_SPEED;
	gamestate.ball.yVel = BALL_SPEED;


	float NET_TICK_RATE = 30;

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
			check_ipaddress(packet_receive, client_ipaddress);
			identify_player(packet_receive, client_ipaddress, &gamestate);

		}
		printf("TRIED TO SEND ball x: %lf\n", gamestate.ball.x);


		detect_collision(&gamestate);

		if (tick_t1 >= next_net_tick)
		{
			for (i = 0; i < 4; i++)
			{
				if (client_ipaddress[i].port != 0)
				{

					packet_send->data = (void*)&gamestate;
					packet_send->channel = -1;
					packet_send->len = sizeof(gamestate);
					packet_send->maxlen = packet_send->len+10;

					
					packet_send->address.host = client_ipaddress[i].host;
					packet_send->address.port = client_ipaddress[i].port;


					if (SDLNet_UDP_Send(server_socket, -1, packet_send)==0)
					{
						printf("failed to send from server/n");
					}

				}

			}
			next_net_tick += net_tick_interval;
			
		}

		

	}

	SDLNet_FreePacket(packet_send);
	SDLNet_FreePacket(packet_receive);
	SDLNet_Quit();
}