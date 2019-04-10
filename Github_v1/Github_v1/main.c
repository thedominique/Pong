#include <stdio.h>
#include <SDL.h>
#include <SDL_net.h>
#include <string.h>
#include <stdbool.h>
#include "game_state.h"
#include "constants.h"
#include "render.h"


void game_Loop(UDPsocket* serversock);
bool address_equal(IPaddress a, IPaddress b);
void print_ip(int ip);	// bara för att identifiera att rätt maskin hittad

int main(int argc, char** argv[]) {
	IPaddress server_ip;
	UDPsocket serversock;
	SDL_Window* window;
	SDL_Renderer* render;
	int hesitation = 1;

	do {
		printf("CHOSEDFSAKPOFASK\n");
		printf("Client = 0 Server = 3 :L);)L)L)\n");
		if (hesitation == 0)
		{
			printf("CLIENT OOF\n");
			// INSERT CLIENT FUNCTION
		}
		else if (hesitation == 3)
		{
			printf("SERVER OOF\n");
			// INSERT SERVER FUNCTION
		}
		
	} while (hesitation != 1);

	SDL_Init(SDL_INIT_EVERYTHING);

	SDLNet_Init();

	init_Window(&window, &render);

	SDLNet_ResolveHost(&server_ip, NULL, 1234);

	serversock = SDLNet_UDP_Open(1234);
	if (!serversock)
	{
		printf("SDLNet_UDP_Open: %s\n", SDL_GetError());
		exit(2);
	}

	game_Loop(&serversock, render);

	SDLNet_UDP_Close(serversock);
	SDLNet_Quit();
	SDL_Quit();

	return 0;
}

void game_Loop(UDPsocket* serversock, SDL_Renderer* render) {
	IPaddress client_addr[3] = { 0, 0, 0 };
	//SDL_Event event;

	int which_player = 0;
	int i = 0, new_client = 0;
	float p_xPos = 0, p_yPos = 0;
	float ball_xPos, ball_yPos;
	//float ballSpeed = BALL_SPEED; 
	float PADDLE_INIT_Y0 = SCREEN_H / 2;

	//float paddleSpeed = PLAYER_SPEED;
	gamestate_t gamestate;
	Players_t player[2] = {0,0};
	UDPpacket *packet;
	/*long* ticks_per_sec = 0;
	long* tick_t0 = 0;
	long* next_net_tick = 0;
	long* net_tick_interval = 0;*/


	packet = SDLNet_AllocPacket(1024);

	p_yPos = PADDLE_INIT_Y0;
	while (1)
	{
		long*  tick_t1 = SDL_GetPerformanceCounter();
		void init_tick();
		double dt = ((*tick_t1) - (*tick_t0)) / (double)(*ticks_per_sec);

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
			gamestate_t* client_packet = (gamestate_t*)packet->data;
			//printf("Du har fått ett paket som säkert innehåller något bra\n");
			p_yPos = client_packet->p_yPos;
			p_xPos = client_packet->p_xPos;
			ball_xPos = client_packet->ball_xPos;
			ball_yPos = client_packet->ball_yPos;
			//which_player = player->which_player;
		}

		if ((*tick_t1) >= (*next_net_tick))
		{
			// Stoppa in saker här
			UDPpacket packet;
			gamestate_t client_packet;
			client_packet.p_yPos = p_yPos;
			client_packet.p_xPos = p_xPos;
			//server_packet.ball_xPos = ball_xPos;
			//server_packet.ball_yPos = ball_yPos;
			client_packet.which_player = which_player;

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
				(*next_net_tick) += (*net_tick_interval);
			}
		}
		windowRendering(render, &gamestate, player);
		(*tick_t0) = (*tick_t1);
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