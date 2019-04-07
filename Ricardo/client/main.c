#if 0
#!/bin/sh

gcc - Wall `sdl-config --cflags` udpc.c - o udpc `sdl-config --libs` - lSDL_net

exit
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL_net.h"
#define BALL_WIDTH	20
#define BALL_HEIGHT	20
#define BALL_SPEED 0.5
#define NET_TICK_RATE 20


#define SCREEN_W  1000
#define SCREEN_H  500

float PADDLE_SPEED_X = 10;
float PADDLE_SPEED_Y = 10;
float BALL_INIT_X0 = SCREEN_W / 2;
float BALL_INIT_Y0 = SCREEN_H / 2;
float PADDLE_INIT_Y0 = 0;
const double PAD_WIDTH = 20;
const double PAD_HEIGHT = 100;
const double PAD_PAD = 0.02;

typedef struct client_packet
{
	float ball_yPos;
	float ball_xPos;
	float ball_dvX;
	float ball_dvY;
	float p_yPos;
	float p_xPos;
	int which_player;
	SDL_bool press;
} client_packet_t;

void game_Logic(SDL_Renderer* renderer, UDPsocket* udpsock, IPaddress server_addr, SDL_Window* window);

int main(int argc, char **argv)
{
	UDPsocket clientsock;
	IPaddress ipaddress;
	//int quit, p_xPos = 0, p_yPos = 0;
	//char buf[1600];

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("SDL_Init: %s\n", SDL_GetError());
		exit(2);
	}
	SDLNet_Init();
	/* Open a socket on random port */
	clientsock = SDLNet_UDP_Open(1666);
	if (!clientsock)
	{
		printf("SDLNet_UDP_Open: %s\n", SDL_GetError());
		exit(2);
	}

	/* Resolve server name  */
	SDLNet_ResolveHost(&ipaddress, "192.168.1.150", 1234);

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

	game_Logic(renderer, &clientsock, ipaddress, window);
	SDLNet_Quit();

	return EXIT_SUCCESS;
}

void game_Logic(SDL_Renderer * renderer, UDPsocket * clientsock, IPaddress server_addr, SDL_Window* window) {
	client_packet_t client_packet;
	UDPpacket* packet;
	int which_player = 0;
	float quit, p1_posX0 = 0, p1_posY0 = PADDLE_INIT_Y0, p_posY = 0, p_posX = 0;
	float p2_posX0 = SCREEN_W, p2_posY0 = SCREEN_H;
	float p_posX1 = 0, p_posY1 = 0;
	float ball_x = BALL_INIT_X0, ball_y = BALL_INIT_Y0;
	float pad2_y = 0;
	SDL_Event event;
	SDL_bool buttonPress = SDL_FALSE;

	long ticks_per_sec = SDL_GetPerformanceFrequency();
	long tick_t0 = SDL_GetPerformanceCounter();

	long next_net_tick = tick_t0;
	long net_tick_interval = (1 / NET_TICK_RATE) * ticks_per_sec;

	/* Allocate memory for the packet */
	packet = SDLNet_AllocPacket(512);

	/* Main loop */
	quit = 1;
	while (quit)
	{
		long tick_t1 = SDL_GetPerformanceCounter();
		double dt = (tick_t1 - tick_t0) / (double)ticks_per_sec;

		while (SDL_PollEvent(&event))
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
		}

		if (SDLNet_UDP_Recv((*clientsock), packet))
		{
			printf("GOTTEM\n");
			client_packet_t* client_packet = (client_packet_t*)packet->data;
			//p_xPos = client_packet->p_xPos;
			//buttonPress = client_packet->press;
			p_posY1 = client_packet->p_yPos;
			p_posX1 = client_packet->p_xPos;
			which_player = client_packet->which_player;
			printf("%f %f\n", p_posX1, p_posY1);
			printf("Player %d\n", which_player);

			if (buttonPress)
			{
				if (which_player == 1)
				{
					p1_posX0 += p_posX1;
					p1_posY0 += p_posY1;
				}
				else if (which_player == 2)
				{
					p2_posX0 += p_posX1;
					p2_posY0 += p_posY1;
				}
				else if (which_player == 3)
				{
					printf("Hej\n");
				}
			}
		}

		const Uint8* state = SDL_GetKeyboardState(NULL);
		if (state[SDL_SCANCODE_W] && p_posY > 0)
		{
			p_posY -= PADDLE_SPEED_Y * dt;
			//printf("HaHa");
			//printf("%f\n", p_posY);
		}
		if (state[SDL_SCANCODE_S] && p_posY < SCREEN_H - PAD_HEIGHT)
		{
			p_posY +=PADDLE_SPEED_Y * dt;
			//printf("HEHE");
			//printf("%f\n", p_posY);
		}
		if (state[SDL_SCANCODE_A] && p_posX > 0)
		{
			p_posX -=PADDLE_SPEED_X * dt;
			//printf("HEHE");
			//printf("%f\n", p_posY);
		}
		if (state[SDL_SCANCODE_D] && p_posX < SCREEN_W - PAD_WIDTH)
		{
			p_posX += PADDLE_SPEED_X * dt;
			//printf("HEHE");
			//printf("%f\n", p_posY);
		}


		if (tick_t1 >= next_net_tick)
		{
			client_packet.p_yPos = p_posY;
			client_packet.p_xPos = p_posX;
			client_packet.which_player = which_player;
			
			//printf("WEEE%f\n", client_packet.p_yPos);
			packet->data = (void*)&client_packet;
			packet->address = server_addr;
			packet->channel = -1;
			packet->len = sizeof(client_packet);
			packet->maxlen = packet->len;
			SDLNet_UDP_Send(*clientsock, -1, packet);
			//printf("Sent!\n");
			next_net_tick += net_tick_interval;
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 40, 0xFF);

		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

		// Draw pad 1.
		SDL_Rect fillRect = { p1_posX0 + p_posX1, p1_posY0 + p_posY1, PAD_WIDTH, PAD_HEIGHT };
		SDL_RenderFillRect(renderer, &fillRect);

		// Draw pad 2.
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_Rect fillRect2 = { p2_posX0 - PAD_WIDTH + p_posX1, p2_posY0 - PAD_HEIGHT + p_posY1, PAD_WIDTH, PAD_HEIGHT };
		SDL_RenderFillRect(renderer, &fillRect2);

		// Draw ball.
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_Rect fillRect3 = { ball_x , ball_y, BALL_WIDTH , BALL_HEIGHT };
		SDL_RenderFillRect(renderer, &fillRect3);

		tick_t0 = tick_t1;
		SDL_RenderPresent(renderer);
	}
	SDLNet_FreePacket(&packet);
	return;
}