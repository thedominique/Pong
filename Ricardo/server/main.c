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

typedef struct player {
	int p_yPos;
	int p_xPos;
	int which_Player;
	//SDL_bool isPlayer1;
} Player;

typedef struct ball {	
	float ball_yPos;
	float ball_xPos;
	float ball_dvX;
	float ball_dvY;
} Ball;

void game_Loop(UDPsocket* serversock);
void print_ip(int ip);	// bara för att identifiera att rätt maskin hittad

int main(int argc, char** argv[]){
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

void game_Loop(UDPsocket* serversock){
	Player player = {0};
	UDPpacket *packet;
	IPaddress client_addr[3] = { 0,0,0};
	//SDL_Event event;
	char choice[10];
	packet = SDLNet_AllocPacket(1024);

	int i = 0;

	int player_xPos;
	int player_yPos;


	float ballSpeed = BALL_SPEED;
	float paddleSpeed = PLAYER_SPEED;

	/*while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_WINDOWEVENT_CLOSE:
		{
			if (window)
			{
				SDL_DestroyWindow(window);
				window = NULL;
				//done = 1;
			}
		}
		break;
		case SDL_KEYDOWN:
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				//done = 1;
				break;
			}
		}
		break;
		case SDL_QUIT:
			//quit out of the game
			//done = 1;
			break;
		}
	}*/

	player_xPos = 65;
	player_yPos = 30;
	//client_addr[0] = packet[0].address;
	//client_addr[1] = packet[1].address;
	while (1)
	{

		for (int j = 0; j < 3; j++)
		{
			if (client_addr[j].host == 0)
			{
				printf("No client(s) joined\n");
				//client_addr[j] = packet[j].address;
			}
			else {
				printf("Found new client\n");
				client_addr[j] = packet[j].address;
				//player[j].which_Player = j;
				print_ip(client_addr[j].host);
				printf("Client(s): %d\n", j+1);
				break;
			}
		}
			player.p_xPos = player_xPos++;
			player.p_yPos = player_yPos++;
			//i = 0;
				player_xPos++;
				player_yPos++;
			for (i = 0; client_addr[i].host != 0 && i < 2; i++)
			{
				printf("IM IN!\n");
				packet->data = (void *)&player;
				packet->address = client_addr[i];
				packet->channel = -1;
				packet->len = sizeof(player);
				packet->maxlen = packet->len;
				packet->address.host = client_addr[i].host;	// Set the destination host
				packet->address.port = client_addr[i].port;	// And destination port
				//packet->len = strlen((char *)packet->data) + 1;
				SDLNet_UDP_Send(*serversock, -1, packet); // This sets the p->channel

			}
			//printf("utanfor\n");
			// Quit if packet contains "quit"
			if (!strcmp((char *)packet->data, "quit"))
			{ 
				return;
			}
		//}
		//else if (!strcmp(choice, "ta emot"))
		//{
			if (SDLNet_UDP_Recv(*serversock, packet))
			{
				printf("Du har fått ett paket som säkert innehåller något bra\n");
				client_addr[i].host = packet->address.host;
				client_addr[i].port = packet->address.port;
			}

		//}
		else
		{
			//printf("NOT A VALID ASS DUDE\n");
		}
			SDL_Delay(1000);
		/*
		//Clear the screen (to blue)
		SDL_RenderClear(renderer);

		//set the drawing color to blue
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

		//set the drawing color to white
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

		SDL_RenderPresent(renderer);
		printf("Ding!\n");

		//don't burn up the CPU
		//SDL_Delay(1000 / 60);
		*/
	}
	SDLNet_FreePacket(packet);
}

void print_ip(int ip){
	unsigned char bytes[4];
	bytes[0] = ip & 0xFF;
	bytes[1] = (ip >> 8) & 0xFF;
	bytes[2] = (ip >> 16) & 0xFF;
	bytes[3] = (ip >> 24) & 0xFF;
	printf("%d.%d.%d.%d\n", bytes[3], bytes[2], bytes[1], bytes[0]);
}
