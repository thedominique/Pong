#include <stdio.h>
#include <SDL.h>
#include <SDL_net.h>
#include <string.h>
#include <stdbool.h>

#define SCREEN_W 1000
#define SCREEN_H 500
#define PAD_W 40
#define PAD_H 200

#define BALL_SPEED 5
#define PLAYER_SPEED 5
#define LOCAL_PORT 1234
#define NET_TICK_RATE 30

typedef struct servPacket {
	float ball_yPos;
	float ball_xPos;
	float ball_dvX;
	float ball_dvY;
	float p_yPos;
	float p_xPos;
	int which_player;
} server_packet_t;

void game_Loop(UDPsocket* serversock);
bool address_equal(IPaddress a, IPaddress b);
void print_ip(int ip);	// bara för att identifiera att rätt maskin hittad

int main(int argc, char** argv[]) {
	IPaddress server_ip;
	//IPaddress client_ip;
	UDPsocket serversock;
	//UDPpacket *packet;
	//char choice[10];

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
	server_packet_t server_packet;
	UDPpacket *packet;
	IPaddress client_addr[3] = { 0, 0, 0 };
	//SDL_Event event;
	char choice[10];
	packet = SDLNet_AllocPacket(1024);

	int which_player = 0, i = 0;
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
	long tick_t1 = SDL_GetPerformanceCounter();
	double dt = (tick_t1 - tick_t0) / (double)ticks_per_sec;

		if (tick_t1 >= next_net_tick)
		{
		if (SDLNet_UDP_Recv(*serversock, packet))
		{
			if (address_equal(packet->address, client_addr[0]))
			{
				printf("Hej Client 1!\n");
			}
			else if (address_equal(packet->address, client_addr[1]))
			{
				printf("Hej Client 2!\n");
			}
			else if (address_equal(packet->address, client_addr[2]))
			{
				printf("Hej Client 3!\n");
			}
			else {
				printf("Valkommen!\n");
				client_addr[0] = packet->address;
			}
			server_packet_t* player = (server_packet_t*)packet->data;
			//printf("Du har fått ett paket som säkert innehåller något bra\n");
			p_yPos = player->p_yPos;
			//printf("height: %d\n", p_yPos);
		/*	for (int j = 0; j<3; j++)
			{
				if (!address_equal(packet->address, client_addr[j]) || !address_equal(packet->address, client_addr[j-1]))
				{
				printf("New Client!\n");
				client_addr[j] = packet->address;
				break;
				}
				else {
					printf("Old Client\n");
				}
			}
			for (int j = 0; j < 3; j++)
			{
				if (client_addr[j].host == 0)
				{

					if (address_equal(client_addr[j], packet->address))
					{
						printf("Old client!\n");
					}
					printf("Found new client\n");
					client_addr[j] = packet[j].address;
					print_ip(client_addr[j].host);
					printf("Client(s): %d\n", j + 1);
				}
				else {
					printf("No client(s) joined\n");
				//client_addr[j] = packet[j].address;
				}
			}
			server_packet_t* player = (server_packet_t*)packet->data;
			//printf("Du har fått ett paket som säkert innehåller något bra\n");
			p_yPos = player->p_yPos;
			//printf("height: %d\n", p_yPos);*/

		}
			server_packet.p_yPos = p_yPos;





			for (i = 0; i < 3; i++)
			{
				printf("IM IN!\n");
				packet->data = (void *)&server_packet;
				packet->address = client_addr[i];
				packet->channel = -1;
				packet->len = sizeof(server_packet);
				packet->maxlen = packet->len;
				packet->address.host = client_addr[i].host;	// Set the destination host
				packet->address.port = client_addr[i].port;	// And destination port
				//packet->len = strlen((char *)packet->data) + 1;
				SDLNet_UDP_Send(*serversock, -1, packet); // This sets the p->channel
			}
		next_net_tick += net_tick_interval;
		tick_t0 = tick_t1;
		}
	}
	SDLNet_FreePacket(packet);
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