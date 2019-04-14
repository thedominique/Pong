#include <stdio.h>
#include <SDL.h>
#include <SDL_net.h>
#include <string.h>
#include <stdbool.h>
#include "constants.h"
#include "functions.h"

#define BALL_SPEED 5
#define PLAYER_SPEED 1

void game_Loop();
void wallhit(ball_packet ball[]);
void moveball(ball_packet ball[], double dt);
void print_ip(int ip);	// bara för att identifiera att rätt maskin hittad

int serverState(void) {

	SDL_Init(SDL_INIT_EVERYTHING);
	SDLNet_Init();


	game_Loop();
	SDLNet_Quit();
	SDL_Quit();

	return 0;
}

void game_Loop() {
	IPaddress client_addr[3] = { 0 };
	IPaddress server_addr;
	UDPsocket serversock;
	ball_packet ball[1];
	UDPpacket *packet;

	int which_player = 0, i = 0, new_client = 0;
	int yangle = 0, h_dir = 0, v_dir = 0;
	float p_xPos = 0, p_yPos = 0;
	float ball_xPos = BALL_INIT_X0, ball_yPos = 50;

	long ticks_per_sec = SDL_GetPerformanceFrequency();
	long tick_t0 = SDL_GetPerformanceCounter();

	long next_net_tick = tick_t0;
	long net_tick_interval = (1 / NET_TICK_RATE) * ticks_per_sec;

	serversock = SDLNet_UDP_Open(1234);
	SDLNet_ResolveHost(&server_addr, "NULL", 1234);
	if (!serversock)
	{
		printf("SDLNet_UDP_Open: %s\n", SDL_GetError());
		exit(2);
	}

	packet = SDLNet_AllocPacket(1600);
	while (1)
	{
		long tick_t1 = SDL_GetPerformanceCounter();
		double dt = (tick_t1 - tick_t0) / (double)ticks_per_sec;

		if (SDLNet_UDP_Recv(serversock, packet))
		{

			if (address_equal(packet->address, client_addr[0]))
			{
				client_addr[0] = packet->address;
				which_player = 1;
			}
			else if (address_equal(packet->address, client_addr[1]))
			{
				client_addr[1] = packet->address;
				which_player = 2;
			}
			else {
				printf("Welcome player %d", new_client);
				client_addr[new_client] = packet->address;
				new_client++;
			}


			IP_packet_t* client_packet = (IP_packet_t*)packet->data;

			p_xPos = client_packet->pad_X;
			//printf("playerX: %f\n", p_xPos);
			p_yPos = client_packet->pad_Y;
			//printf("playerY: %f\n", p_yPos);

			ball_xPos = client_packet->ball_xPos;
			//printf("Ball X Position: %f\n", ball_xPos);
			ball_yPos = client_packet->ball_yPos;
			//printf("Ball Y Position: %f\n", ball_yPos);
			yangle = client_packet->yangle;
			//printf("ANGLE: %d\n", yangle);

			h_dir = client_packet->h_dir;
			//printf("HORIZONTAL: %d\n", h_dir);
			v_dir = client_packet->v_dir;
			//printf("VERTICAL: %d\n", v_dir);

			ball[0].xPos = ball_xPos;
			ball[0].yPos = ball_yPos;
			ball[0].h_direction = h_dir;
			ball[0].v_direction = v_dir;
			ball[0].yangle = yangle;
			//}
		}
		moveball(ball, dt);
		wallhit(ball);

		if (tick_t1 >= next_net_tick)
		{
			// Stoppa in saker här
			UDPpacket packet;
			IP_packet_t client_packet = { 0 };

			client_packet.pad_X = p_xPos;
			client_packet.pad_Y = p_yPos;
			client_packet.ball_xPos = ball[0].xPos;
			client_packet.ball_yPos = ball[0].yPos;
			client_packet.yangle = ball[0].yangle;
			client_packet.h_dir = ball[0].h_direction;
			client_packet.v_dir = ball[0].v_direction;
			client_packet.which_player = which_player;

			for (i = 0; i < 2; i++)
			{
				if (client_addr[i].port == 0) {
					//printf("Client #%d\n", i);
					continue;
				}
				//printf("IM IN!\n");
				packet.address = client_addr[i];
				packet.data = (void*)&client_packet;
				packet.channel = -1;
				packet.len = sizeof(client_packet);
				packet.maxlen = packet.len;
				if (SDLNet_UDP_Send(serversock, -1, &packet) == 0)
				{
					printf("SDLNet_UDP_Send: failed %s\n", SDLNet_GetError());
				}
			}
			next_net_tick += net_tick_interval;
		}
		tick_t0 = tick_t1;
		SDLNet_FreePacket(packet);
	}
	SDLNet_UDP_Close(serversock);
}

