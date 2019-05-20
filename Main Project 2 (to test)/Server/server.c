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
#include "init_server_values.h"

void check_gameover(GameState *gamestate)
{
	int i = 0;
	int check = 0;

	for (i = 0; i < 3; i++)
	{
		if (gamestate->players[i].lives == 0)
		{
			check++;
		}
	}
	if (check >= 2)
	{
		gamestate->gameover = 1;
	}
}



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
	init_server_values(&gamestate);


	for (i = 0; i < 3; i++)
	{
		gamestate.keys[i].up = 0;
		gamestate.keys[i].down = 0;
	}

	float NET_TICK_RATE = 50;

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
			get_client_values(packet_receive, client_ipaddress, &gamestate);

		}
		//printf("TRIED TO SEND ball x: %lf\n", gamestate.ball.x);
		//printf("Lives player 0: %d\n", gamestate.players[0].lives);
		//printf("gamestate player[0].x: %lf\n", gamestate.players[0].x);
		//printf("play again: %d %d %d \n",gamestate.players[0].playAgain, gamestate.players[1].playAgain, gamestate.players[2].playAgain);
		//printf("quit: %d %d %d\n", gamestate.players[0].quit, gamestate.players[1].quit, gamestate.players[2].quit);
		//printf("playercounter: %d ", gamestate.playerCounter);
		//printf("gamestarted: %d %d %d \n", gamestate.players[0].gameStarted, gamestate.players[1].gameStarted, gamestate.players[2].gameStarted);
		//printf("game over: %d \n", gamestate.gameover);

		//HANTERAR COLLISION OCH KEY PRESSES
		
		if (gamestate.gameover)
		{
			reset_game(&gamestate);
		}

		if (gamestate.gameStarted && !gamestate.gameover)
		{
			detect_collision(&gamestate);
			check_gameover(&gamestate);
			key_handler(&gamestate);
			key_handler(&gamestate);
		}


		if (!gamestate.gameStarted)
		{
			check_players_started(&gamestate);
		}

		//printf("server game over: %d\n", gamestate.gameover);
		

		if (tick_t1 >= next_net_tick)
		{
			for (i = 0; i < 4; i++)
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