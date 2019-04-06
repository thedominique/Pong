#if 0
#!/bin/sh

gcc - Wall `sdl-config --cflags` udpc.c - o udpc `sdl-config --libs` - lSDL_net

exit
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL_net.h"
#define BALL_WIDTH	0.015
#define BALL_HEIGHT	0.05

int SCREEN_W = 1000;
int SCREEN_H = 500;

float BALL_INIT_X0 = 0.5 - BALL_WIDTH / 2;
float BALL_INIT_Y0 = 0.5 - BALL_HEIGHT / 2;
const double PAD_WIDTH = 0.02;
const double PAD_HEIGHT = 0.15;
const double PAD_PAD = 0.02;

typedef struct Paddle
{
	int p_xPos, p_yPos;
	//int xVel, yVel;
} Player;

typedef struct ball {
	float ball_yPos;
	float ball_xPos;
	float ball_dvX;
	float ball_dvY;
} Ball;


void game_Logic(SDL_Renderer* renderer, UDPsocket* udpsock, IPaddress server_addr);

int main(int argc, char **argv)
{
	UDPsocket clientsock;
	IPaddress ipaddress;
	//int quit, p_xPos = 0, p_yPos = 0;
	//char buf[1600];

	SDLNet_Init();
	/* Open a socket on random port */
	clientsock = SDLNet_UDP_Open(1666);
	if (!clientsock)
	{
		printf("SDLNet_UDP_Open: %s\n", SDL_GetError());
		exit(2);
	}

	/* Resolve server name  */
	SDLNet_ResolveHost(&ipaddress, "192.168.1.185", 1234);

	SDL_Window* window = SDL_CreateWindow(
	"Ultra pong",
	SDL_WINDOWPOS_UNDEFINED,
	SDL_WINDOWPOS_UNDEFINED,
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

	game_Logic(renderer, &clientsock, ipaddress);
	SDLNet_Quit();

	return EXIT_SUCCESS;
}

void game_Logic(SDL_Renderer * renderer, UDPsocket * clientsock, IPaddress server_addr){
	UDPpacket packet;
	int quit, p_xPos = 0, p_yPos = 0;
	char buf[1600];
	float ball_x = BALL_INIT_X0, ball_y = BALL_INIT_Y0;
	float pad1_y = 0, pad2_y = 0;

	/* Allocate memory for the packet */
	//packet = SDLNet_AllocPacket(1024);
	packet.data = (void*)buf;
	packet.maxlen = sizeof(buf);

	/* Main loop */
	quit = 1;
	while (quit)
	{
		if (SDLNet_UDP_Recv((*clientsock), &packet))
		{
			Player* player = (Player*)packet.data;
			p_xPos = player->p_xPos;
			p_yPos = player->p_yPos;
			printf("%d %d\n", p_xPos, p_yPos);

		}
		printf("Fill the buffer\n>");

		gets((char *)packet.data);

		packet.address.host = server_addr.host;	/* Set the destination host */
		packet.address.port = server_addr.port;	/* And destination port */

		packet.len = strlen((char *)packet.data) + 1;
		SDLNet_UDP_Send((*clientsock), -1, &packet); /* This sets the packet->channel */
		/* Quit if packet contains "quit" */
		if (!strcmp((char *)packet.data, "quit"))
			quit = 1;

		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

		// Draw pad 1.
		SDL_Rect fillRect = { PAD_PAD * SCREEN_W, pad1_y*SCREEN_H, PAD_WIDTH * SCREEN_W, PAD_HEIGHT * SCREEN_H };
		SDL_RenderFillRect(renderer, &fillRect);

		// Draw pad 2.
		SDL_Rect fillRect2 = { (1.0 - PAD_PAD - PAD_WIDTH) * SCREEN_W, pad2_y*SCREEN_H, PAD_WIDTH * SCREEN_W, PAD_HEIGHT * SCREEN_H };
		SDL_RenderFillRect(renderer, &fillRect2);

		// Draw ball.
		SDL_Rect fillRect3 = { ball_x * SCREEN_W, ball_y*SCREEN_H, BALL_WIDTH * SCREEN_W, BALL_HEIGHT * SCREEN_H };
		SDL_RenderFillRect(renderer, &fillRect3);

		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderPresent(renderer);

		//SDL_Delay(1000/60);

	}
	SDLNet_FreePacket(&packet);
	return;
}
