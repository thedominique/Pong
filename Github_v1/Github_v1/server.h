#pragma once
#ifndef  server_h
#define server_h

#include <stdio.h>
#include <SDL.h>
#include <SDL_net.h>
#include <string.h>
#include <stdbool.h>
#include "constants.h"

#define BALL_SPEED 5
#define PLAYER_SPEED 1

typedef struct clientpacket {
	float ball_yPos;
	float ball_xPos;
	float ball_dvX;
	float ball_dvY;
	float p_yPos, p_xPos;
	int which_player;
	int p_horizontal_direction;
	int p_vertical_direction;
	int p_yangle;
	SDL_bool press;
} client_packet_t;

void game_Loop(UDPsocket* serversock);
bool address_equal(IPaddress a, IPaddress b);
void print_ip(int ip);	// bara för att identifiera att rätt maskin hittad

void serverState(void) {
	IPaddress server_ip;
	UDPsocket serversock;

	SDL_Init(SDL_INIT_EVERYTHING);
	SDLNet_Init();

	SDLNet_ResolveHost(&server_ip, NULL, 1234);

	serversock = SDLNet_UDP_Open(1234);
	if (!serversock)
	{
		printf("SDLNet_UDP_Open: %s\n", SDL_GetError());
		exit(2);
	}

	game_Loop(&serversock);

	SDLNet_UDP_Close(serversock);
	SDLNet_Quit();
	SDL_Quit();

	return 0;
}

void game_Loop(UDPsocket* serversock) {
	IPaddress client_addr[3] = { 0, 0, 0 };
	//SDL_Event event;

	int which_player = 0, i = 0, new_client = 0;
	float p_xPos = 0, p_yPos = 0;
	float ball_xPos = SCREEN_W / 2, ball_yPos = SCREEN_H / 2;
	float ballSpeed = BALL_SPEED;
	float paddleSpeed = PLAYER_SPEED;
	float PADDLE_INIT_Y0 = SCREEN_H / 2;

	int p_horizontal_direction = 0;
	int p_vertical_direction = 0;
	int p_yangle = 0;


	UDPpacket *packet;

	long ticks_per_sec = SDL_GetPerformanceFrequency();
	long tick_t0 = SDL_GetPerformanceCounter();

	long next_net_tick = tick_t0;
	long net_tick_interval = (1 / NET_TICK_RATE) * ticks_per_sec;
	packet = SDLNet_AllocPacket(1024);

	p_yPos = PADDLE_INIT_Y0;
	while (1)
	{
		long tick_t1 = SDL_GetPerformanceCounter();
		double dt = (tick_t1 - tick_t0) / (double)ticks_per_sec;

		if (SDLNet_UDP_Recv(*serversock, packet))
		{
			printf("time: %f\n", dt);

			if (address_equal(packet->address, client_addr[0]))
			{
				//printf("Hej Client 1!\n");
				which_player = 1;
			}
			else if (address_equal(packet->address, client_addr[1]))
			{
				client_addr[1] = packet->address;
				//printf("Hej Client 2!\n");
				which_player = 2;
			}
			else if (address_equal(packet->address, client_addr[2]))
			{
				client_addr[2] = packet->address;
				//printf("Hej Client 3!\n");
				which_player = 3;
			}
			else if (new_client == 3)
			{
				printf("Full\n");
			}
			else {
				printf("Valkommen client %d!\n", new_client);
				client_addr[new_client] = packet->address;
				//which_player = 1;
				new_client = new_client + 1;
			}
			client_packet_t* client_packet = (client_packet_t*)packet->data;
			//printf("Du har fått ett paket som säkert innehåller något bra\n");
			p_yPos = client_packet->p_yPos;
			p_xPos = client_packet->p_xPos;
			ball_xPos = client_packet->ball_xPos;
			ball_yPos = client_packet->ball_yPos;

			p_yangle = client_packet->p_yangle;
			p_horizontal_direction = client_packet->p_horizontal_direction;
			p_vertical_direction = client_packet->p_vertical_direction;
			which_player = client_packet->which_player;
		}

		if (tick_t1 >= next_net_tick)
		{
			// Stoppa in saker här
			UDPpacket packet;
			client_packet_t client_packet;
			client_packet.p_yPos = p_yPos;
			client_packet.p_xPos = p_xPos;
			client_packet.ball_xPos = ball_xPos;
			client_packet.ball_yPos = ball_yPos;
			client_packet.which_player = which_player;

			client_packet.p_yangle = p_yangle;
			client_packet.p_horizontal_direction = p_horizontal_direction;
			client_packet.p_vertical_direction = p_vertical_direction;

			for (i = 0; i < 2; i++)
			{
				if (client_addr[i].port == 0) {
					printf("Client #%d\n", i);
					continue;
				}
				//printf("IM IN!\n");
				packet.address = client_addr[i];
				packet.data = (void *)&client_packet;
				packet.channel = -1;
				packet.len = sizeof(client_packet);
				packet.maxlen = packet.len;
				if (SDLNet_UDP_Send(*serversock, -1, &packet) == 0)
				{
					printf("SDLNet_UDP_Send: failed %s\n", SDLNet_GetError());
				}
				next_net_tick += net_tick_interval;
			}
		}
		tick_t0 = tick_t1;
		SDLNet_FreePacket(packet);
	}
}

void print_ip(int ip) {

	unsigned char bytes[4];
	bytes[0] = ip & 0xFF;
	bytes[1] = (ip >> 8) & 0xFF;
	bytes[2] = (ip >> 16) & 0xFF;
	bytes[3] = (ip >> 24) & 0xFF;
	printf("%d.%d.%d.%d\n", bytes[3], bytes[2], bytes[1], bytes[0]);
}

bool address_equal(IPaddress a, IPaddress b) {
	return a.host == b.host && a.port == b.port;
}
#endif // ! server_h

