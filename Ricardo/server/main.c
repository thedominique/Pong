#include <stdio.h>
#include <SDL.h>
#include <SDL_net.h>
#include <string.h>
#include <stdbool.h>

#define SCREEN_W 1000
#define SCREEN_H 500
#define BALL_SPEED 5
#define PLAYER_SPEED 1
#define LOCAL_PORT 1234

float NET_TICK_RATE = 30;

typedef struct servPacket {
	float ball_yPos;
	float ball_xPos;
	float ball_dvX;
	float ball_dvY;
	float p_yPos;
	float p_xPos;
	int which_player;
	SDL_bool press;
} server_packet_t;

void game_Loop(UDPsocket* serversock);
bool address_equal(IPaddress a, IPaddress b);
void print_ip(int ip);	// bara för att identifiera att rätt maskin hittad

int main(int argc, char** argv[]) {
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
	float ballSpeed = BALL_SPEED;
	float paddleSpeed = PLAYER_SPEED;
	float PADDLE_INIT_Y0 = SCREEN_H / 2;

	long ticks_per_sec = SDL_GetPerformanceFrequency();
	long tick_t0 = SDL_GetPerformanceCounter();

	long next_net_tick = tick_t0;
	long net_tick_interval = (1 / NET_TICK_RATE) * ticks_per_sec;

	p_yPos = PADDLE_INIT_Y0;
	while (1)
	{
		UDPpacket *packet;
		packet = SDLNet_AllocPacket(1024);
		long tick_t1 = SDL_GetPerformanceCounter();
		double dt = (tick_t1 - tick_t0) / (double)ticks_per_sec;

		if (SDLNet_UDP_Recv(*serversock, packet))
		{
			if (address_equal(packet->address, client_addr[0]))
			{
				printf("Hej Client 1!\n");
				which_player = 1;
			}
			else if (address_equal(packet->address, client_addr[1]))
			{
				client_addr[1] = packet->address;
				printf("Hej Client 2!\n");
				which_player = 2;
			}
			else if (address_equal(packet->address, client_addr[2]) )
			{
				client_addr[2] = packet->address;
				printf("Hej Client 3!\n");
				which_player = 3;
			}
			else {
				printf("Valkommen!\n");
				client_addr[new_client] = packet->address;
				which_player = 1;
				new_client = new_client + 1;
			}
			server_packet_t* player = (server_packet_t*)packet->data;
			//printf("Du har fått ett paket som säkert innehåller något bra\n");
			p_yPos = player->p_yPos;
			p_xPos = player->p_xPos;
			//which_player = player->which_player;
		}

		if (tick_t1 >= next_net_tick)
		{
			UDPpacket packet;
			server_packet_t server_packet;
			server_packet.p_yPos = p_yPos;
			server_packet.p_xPos= p_xPos;
		//	server_packet.ball_xPos = ball_xPos;
		//server_packet.ball_yPos = ball_yPos;
			server_packet.which_player = which_player;

			for (i = 0; i < 3; i++)
			{
				if (client_addr[i].port == 0) {
					continue;
				}
				//printf("IM IN!\n");
				packet.address = client_addr[i];
				packet.data = (void *)&server_packet;
				packet.channel = -1;
				packet.len = sizeof(server_packet);
				packet.maxlen = packet.len;
				if (SDLNet_UDP_Send(*serversock, -1, &packet) == 0)
				{
					printf("SDLNet_UDP_Send: failed %s\n", SDLNet_GetError());
				}
			}
			next_net_tick += net_tick_interval;
		}
		tick_t0 = tick_t1;
	}
	//SDLNet_FreePacket(packet);
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
