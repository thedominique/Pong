#if 0
#!/bin/sh

gcc - Wall `sdl-config --cflags` udpc.c - o udpc `sdl-config --libs` - lSDL_net

exit
#endif

typedef struct player {
	float player_yPos;
	float player_xPos;
	//SDL_bool isPlayer1;
} Player;

typedef struct ball {
	float ball_yPos;
	float ball_xPos;
	float ball_dvX;
	float ball_dvY;
} Ball;

#define SCREEN_W 1000
#define SCREEN_H 500
#define PAD_W 40
#define PAD_H 200

#define BALL_SPEED 5
#define PLAYER_SPEED 5
#define LOCAL_PORT 1234

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SDL_net.h"

void game_Loop(SDL_Renderer* renderer, SDL_Window* window, UDPsocket * clientSock);

int main(int argc, char **argv)
{
	UDPsocket clientSock;
	IPaddress ipaddress;
	//UDPpacket *packet;

	SDL_Init(SDL_INIT_EVERYTHING);
	SDLNet_Init();

	/* Open a socket on random port */
	clientSock = SDLNet_UDP_Open(1666);
	/* Resolve server name  */
	SDLNet_ResolveHost(&ipaddress, "130.229.171.37", 1234);


	if (!clientSock)
	{
		printf("SDLNet_UDP_Open: %s\n", SDL_GetError());
		exit(2);
	}



	SDL_Window* window = SDL_CreateWindow(
		"Ultra pong",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		SCREEN_W,
		SCREEN_H,
		0);

	if (!window) {
		printf("Could not create window: %s\n", SDL_GetError());
		return 1;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		printf("Could not create renderer: %s\n", SDL_GetError());
		return 1;
	}

	game_Loop(renderer, window, &clientSock);

	SDLNet_Quit();

	return EXIT_SUCCESS;
}

void game_Loop(SDL_Renderer* renderer, SDL_Window* window, UDPsocket * clientSock) {
	UDPpacket *packet;
	IPaddress server_addr = {0};
	//SDL_Event event;
	char choice[10];
	packet = SDLNet_AllocPacket(1024);
	SDLNet_ResolveHost(&server_addr, "130.229.171.37", 1234);

	float ballSpeed = BALL_SPEED;
	float paddleSpeed = PLAYER_SPEED;


	int quit;
	//Input

	/* Allocate memory for the packet */
	//packet = SDLNet_AllocPacket(1024);


	/* Main loop */
	quit = 0;
	while (!quit)
	{
		printf("Fill the buffer\n>");
		//scanf("%s", (char*)(*packet).data);
		gets((char*)packet->data);

		packet->address.host = server_addr.host;	/* Set the destination host */
		packet->address.port = server_addr.port;	/* And destination port */

		packet->len = strlen((char *)packet->data) + 1;
		SDLNet_UDP_Send(*clientSock, -1, packet); /* This sets the packet->channel */


		/* Quit if packet contains "quit" */
		if (!strcmp((char *)packet->data, "quit"))
			quit = 1;

		if (SDLNet_UDP_Recv(*clientSock, packet))
		{
			printf("%s\n", (char*)packet->data);
		}

		//Renderer
		//Clear the screen (to blue)
		SDL_RenderClear(renderer);

		//set the drawing color to blue
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

		//set the drawing color to white
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

		SDL_RenderPresent(renderer);
		//printf("Ding!\n");

		//don't burn up the CPU
		//SDL_Delay(1000 / 60);

	}

	SDLNet_FreePacket(packet);
}
