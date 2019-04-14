#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "SDL_net.h"
#include "constants.h"
#include "functions.h"

//#define BALL_WIDTH	20
//#define BALL_HEIGHT	20
#define SCREEN_W  1000
#define SCREEN_H  500

float PAD_SPEED_X = 500;
float PAD_SPEED_Y = 500;
float PAD_INIT_Y0 = 0;
//---------------------------------------------------------------------------------------------------------------------
const double PAD_PAD = 0.02;

void game_Logic(SDL_Renderer* renderer, SDL_Window* window);
//void moveball(int vertical_direction, int horizontal_direction, float ball_y0, float ball_x0, int yangle, double dt);

int clientState(void) {

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("SDL_Init: %s\n", SDL_GetError());
		exit(2);
	}
	SDLNet_Init();

	SDL_Window* window = SDL_CreateWindow(
		"ONLINE PONG",
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

	game_Logic(renderer, window);
	SDLNet_Quit();

	return EXIT_SUCCESS;
}

void game_Logic(SDL_Renderer * renderer, SDL_Window* window) {
	int which_player = 0, new_client = 0, recv = 0;
	int quit, first_time = 0;
	//INIT PLAYER POSITION------------------------------------------------------------------------------------------------------------
	float p_posX1 = 0, p_posY1 = 0; // Updates player position
	float p_posY = 0, p_posX = 0;	// Takes data from packet. Updates X1 position.
									//--------------------------------------------------------------------------------------------------------------------------------
	//BALL POSITION-------------------------------------------------------------------------------------------------------------------
	float ball_x0 = BALL_INIT_X0, ball_y0 = BALL_INIT_Y0;
	//BALL DIRECTION AND DYNAMIC SPEED FACTOR YANGLE----------------------------------------------------------------------------------
	//int horizontal_direction = 1;
	//int vertical_direction = 1;
	//int yangle = 1;

	//Other Stuff---------------------------------------------------------------------------------------------------------------------
	SDL_Event event;
	IPaddress server_addr, client_addr[3] = { 0 };
	UDPpacket* packet;
	UDPsocket clientsock;
	Players player[3] = { 0 };
	ball_packet ball[1];

	long ticks_per_sec = SDL_GetPerformanceFrequency();
	long tick_t0 = SDL_GetPerformanceCounter();

	long next_net_tick = tick_t0;
	long net_tick_interval = (1 / NET_TICK_RATE) * ticks_per_sec;

	/* Allocate memory for the packet */
	packet = SDLNet_AllocPacket(1600);
	/* Open a socket on random port */
	clientsock = SDLNet_UDP_Open(1667); // define port som en macro för att lättare kunna ändra??
	if (!clientsock)
	{
		printf("SDLNet_UDP_Open: %s\n", SDL_GetError());
		exit(2);
	}

	/* Resolve server name  */
	SDLNet_ResolveHost(&server_addr, "192.168.1.185", 1234); // initiera packet för servern

	ball[0].h_direction = 1;
	ball[0].v_direction = 1;
	ball[0].yangle = 2;
	ball[0].xPos = BALL_INIT_X0;
	ball[0].yPos = BALL_INIT_Y0;

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
		//---------------------------------------------------------------------------------------------------------------------
		//RECIEVE PACKET-------------------------------------------------------------------------------------------------------
		if (SDLNet_UDP_Recv(clientsock, packet))
		{
			IP_packet_t* server_packet = (IP_packet_t*)packet->data;
			//which_player = address_equal(packet->address, client_addr, &new_client);

			p_posY1 = server_packet->pad_Y;
			p_posX1 = server_packet->pad_X;
			p_posY1 = server_packet->pad_Y;
			p_posX1 = server_packet->pad_X;
			which_player = server_packet->which_player;
			ball[0].xPos = server_packet->ball_xPos;
			//printf("Ball X: %f\n", ball_x0);
			ball[0].yPos = server_packet->ball_yPos;
			ball[0].h_direction = server_packet->h_dir;
			ball[0].v_direction = server_packet->v_dir;
			ball[0].yangle = server_packet->yangle;
			//("Welcome Player %d\n", which_player);
			if (which_player == 1)
			{
				//sprintf("Move 1\n");
				player[0].xPos = p_posX1;
				player[0].yPos = p_posY1;
				//	printf("Player 1 move!\n");

			}
			else if (which_player == 2)
			{
				player[1].xPos = p_posX1;
				player[1].yPos = p_posY1;
				//	printf("Player 2 move!\n");

			}
			else if (which_player == 3)
			{
				//	printf("Hej\n");
			}
		}
		//---------------------------------------------------------------------------------------------------------------------
		padhit(player, ball);
		//SDL_STATES-----------------------------------------------------------------------------------------------------------poll events?
		const Uint8* state = SDL_GetKeyboardState(NULL);
		if (state[SDL_SCANCODE_W] && p_posY > 0)
		{
			p_posY -= PAD_SPEED_Y * dt;
			//printf("Up: %f\n", p_posY);
		}
		if (state[SDL_SCANCODE_S] && p_posY < SCREEN_H - PAD_HEIGHT)
		{
			p_posY += PAD_SPEED_Y * dt;
		}
		if (state[SDL_SCANCODE_A]/* && p_posX > 0*/)
		{
			p_posX -= PAD_SPEED_X * dt;
		}
		if (state[SDL_SCANCODE_D] && p_posX < SCREEN_W - PAD_WIDTH)
		{
			p_posX += PAD_SPEED_X * dt;
		}
		//---------------------------------------------------------------------------------------------------------------------

		//wallhit(ball);
		//---------------------------------------------------------------------------------------------------------------------
		//SEND PACKAGE---------------------------------------------------------------------------------------------------------

		if (tick_t1 >= next_net_tick)
		{
			// create new server packet locally and insert contents
			IP_packet_t server_packet;
			server_packet.pad_Y = p_posY;
			server_packet.pad_X = p_posX;
			server_packet.ball_xPos = ball[0].xPos;
			server_packet.ball_yPos = ball[0].yPos;
			server_packet.yangle = ball[0].yangle;
			server_packet.h_dir = ball[0].h_direction;
			server_packet.v_dir = ball[0].v_direction;
			// Create packet then fill it with contents from server packet
			UDPpacket packet;
			packet.data = (void*)&server_packet;
			packet.address = server_addr;
			packet.channel = -1;
			packet.len = sizeof(server_packet);
			packet.maxlen = packet.len;
			if (SDLNet_UDP_Send(clientsock, -1, &packet) == 0)
			{
				printf("SDLNet_UDP_Send: failed to send %s\n", SDLNet_GetError());
			}
			next_net_tick += net_tick_interval;
		}
		//---------------------------------------------------------------------------------------------------------------------
		//RENDERER-------------------------------------------------------------------------------------------------------------
		SDL_SetRenderDrawColor(renderer, 0, 0, 40, 0xFF);

		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

		// Draw pad 1.
		SDL_Rect fillRect = { player[0].xPos , player[0].yPos, PAD_WIDTH, PAD_HEIGHT };
		SDL_RenderFillRect(renderer, &fillRect);

		// Draw pad 2.
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_Rect fillRect2 = { (PAD_WIDTH - 5) + player[1].xPos, player[1].yPos, PAD_WIDTH, PAD_HEIGHT };
		SDL_RenderFillRect(renderer, &fillRect2);

		// Draw ball.
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_Rect fillRect3 = { ball[0].xPos , ball[0].yPos, BALL_WIDTH , BALL_HEIGHT };
		SDL_RenderFillRect(renderer, &fillRect3);

		tick_t0 = tick_t1;
		SDL_RenderPresent(renderer);
	}
	SDLNet_FreePacket(packet);
}
