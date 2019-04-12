#pragma once
#ifndef client_h
#define client_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL_net.h"

#define BALL_WIDTH	20
#define BALL_HEIGHT	20
#define SCREEN_W  1000
#define SCREEN_H  500

//float NET_TICK_RATE = 60;
float PAD_SPEED_X = 500;
float PAD_SPEED_Y = 500;
float BALL_INIT_X0 = SCREEN_W / 2;
float BALL_INIT_Y0 = SCREEN_H / 2;
float PAD_INIT_Y0 = 0;
//***********************************************************************************
//const double PAD_WIDTH = 20;
//const double PAD_HEIGHT = 100;
const double PAD_PAD = 0.02;

typedef struct serverpacket
{
	float ball_yPos;
	float ball_xPos;
	float ball_dvX;
	float ball_dvY;
	float p_yPos, p_xPos; // byt till pad_y, pad_x??
	int which_player;
	int p_horizontal_direction;
	int p_vertical_direction;
	int p_yangle;
} server_packet_t;

typedef struct players
{
	float yPos; // använd snake för mer consistency 
	float xPos;
}Players;

void game_Logic(SDL_Renderer* renderer, UDPsocket* udpsock, IPaddress server_addr, SDL_Window* window);
void wallhit(float ball_y0, float ball_x0, int yangle, int horizontal_direction, int vertical_direction);
void padhit(Players player, int vertical_direction, int horizontal_direction, float ball_y0, float ball_x0);
void moveball(int vertical_direction, int horizontal_direction, float ball_y0, float ball_x0, int yangle);

void clientState(void){
	UDPsocket clientsock;
	IPaddress ipaddress;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("SDL_Init: %s\n", SDL_GetError());
		exit(2);
	}
	SDLNet_Init();

	/* Open a socket on random port */
	clientsock = SDLNet_UDP_Open(1667); // define port som en macro för att lättare kunna ändra??
	if (!clientsock)
	{
		printf("SDLNet_UDP_Open: %s\n", SDL_GetError());
		exit(2);
	}

	/* Resolve server name  */
	SDLNet_ResolveHost(&ipaddress, "130.229.176.242", 1234); // initiera packet för servern

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
	int which_player = 0;
	int quit;
	//INIT PLAYER POSITION***********************************************************************************
	float p1_posX0 = 0, p1_posY0 = PAD_INIT_Y0;
	float p2_posX0 = SCREEN_W, p2_posY0 = PAD_INIT_Y0;
	float p_posX1 = 0, p_posY1 = 0; // Updates player position
	float p_posY = 0, p_posX = 0;	// Takes data from packet. Updates X1 position.
//***********************************************************************************
//BALL POSITION**********************************************************************************/
	float ball_x0 = BALL_INIT_X0, ball_y0 = BALL_INIT_Y0;
	float ball_X1, ball_Y1;
	//BALL DIRECTION AND DYNAMIC SPEED FACTOR YANGLE**********************************************************************************/
	int horizontal_direction = 1;
	int vertical_direction = 0;
	int yangle = 3;

	//Other Stuff**********************************************************************************/
	SDL_Event event;
	SDL_bool buttonPress = SDL_FALSE;
	UDPpacket* packet;
	Players player[3] = { 0,0,0 };

	long ticks_per_sec = SDL_GetPerformanceFrequency();
	long tick_t0 = SDL_GetPerformanceCounter();

	long next_net_tick = tick_t0;
	long net_tick_interval = (1 / NET_TICK_RATE) * ticks_per_sec;

	/* Allocate memory for the packet */
	packet = SDLNet_AllocPacket(1024);
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
		//***********************************************************************************
		//RECIEVE PACKET***********************************************************************************
		if (SDLNet_UDP_Recv((*clientsock), packet))
		{
			server_packet_t* server_packet = (server_packet_t*)packet->data;
			p_posY1 = server_packet->p_yPos;
			p_posX1 = server_packet->p_xPos;
			which_player = server_packet->which_player;	// UNPACK VALUE TO CORRESPONDING
														// IPADDRESS WHICH THE SERVER SORTS OUT
			printf("Welcome Player %d\n", which_player);

			if (which_player == 1)
			{
				player[0].xPos = p_posX1;
				player[0].yPos = p_posY1;
				printf("Player 1 move!\n");
			}
			else if (which_player == 2)
			{
				player[1].xPos = p_posX1;
				player[1].yPos = p_posY1;
				printf("Player 2 move!\n");
			}
			else if (which_player == 3)
			{
				printf("Hej\n");
			}
			ball_x0 = server_packet->ball_xPos;
			ball_y0 = server_packet->ball_yPos;
			horizontal_direction = server_packet->p_horizontal_direction;
			vertical_direction = server_packet->p_vertical_direction;
			yangle = server_packet->p_yangle;
		}
		//***********************************************************************************
		//SDL_STATES*********************************************************************************** // poll events?
		const Uint8* state = SDL_GetKeyboardState(NULL);
		if (state[SDL_SCANCODE_W] && p_posY > 0)
		{
			p_posY -= PAD_SPEED_Y * dt;

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
		//***********************************************************************************



		//***********************************************************************************
		//SEND PACKAGE***********************************************************************************
		if (tick_t1 >= next_net_tick)
		{

			// create new server packet locally and insert contents
			server_packet_t server_packet;

			server_packet.p_yPos = p_posY;
			server_packet.p_xPos = p_posX;
			server_packet.which_player = which_player;

			server_packet.p_yangle = yangle;
			server_packet.p_horizontal_direction = horizontal_direction;
			server_packet.p_vertical_direction = vertical_direction;

			server_packet.ball_yPos = ball_x0;
			server_packet.ball_xPos = ball_y0;



			// Create packet then fill it with contents from server packet
			UDPpacket packet;
			//printf("WEEE%f\n", client_packet.p_yPos);
			packet.data = (void*)&server_packet;
			packet.address = server_addr;
			packet.channel = -1;
			packet.len = sizeof(server_packet);
			packet.maxlen = packet.len;
			//printf("Sent!\n");
			SDLNet_UDP_Send(*clientsock, -1, &packet);
			next_net_tick += net_tick_interval;
		}
		//***********************************************************************************
		//RENDERER***********************************************************************************
		SDL_SetRenderDrawColor(renderer, 0, 0, 40, 0xFF);

		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

		// Draw pad 1.
		SDL_Rect fillRect = { p1_posX0 + player[0].xPos , p1_posY0 + player[0].yPos, PAD_WIDTH, PAD_HEIGHT };
		SDL_RenderFillRect(renderer, &fillRect);

		// Draw pad 2.
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_Rect fillRect2 = { (p2_posX0 - PAD_WIDTH - 5) + player[1].xPos, (p2_posY0)+player[1].yPos, PAD_WIDTH, PAD_HEIGHT };
		SDL_RenderFillRect(renderer, &fillRect2);

		// Draw ball.
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_Rect fillRect3 = { ball_x0 , ball_y0, BALL_WIDTH , BALL_HEIGHT };
		SDL_RenderFillRect(renderer, &fillRect3);

		tick_t0 = tick_t1;
		SDL_RenderPresent(renderer);
		//SDLNet_FreePacket(&packet);
	}
}



void moveball(int vertical_direction, int horizontal_direction, float ball_y0, float ball_x0, int yangle, double dt){

			if (horizontal_direction == 2)
			{
				ball_x0 -= 3 * dt;

				//SDL_Delay(3);
			}

			if (horizontal_direction == 1)
			{
				ball_x0 += 3 * dt;

				//SDL_Delay(3);
			}

			if (vertical_direction == 2)
			{

				ball_y0 += yangle * dt;
				//SDL_Delay(3);
			}

			if (vertical_direction == 1)
			{

				ball_y0 -= yangle * dt;
				//SDL_Delay(3);
			}

		}

//Change ball direction***********************************************************************************
void wallhit(float ball_y0, float ball_x0, int yangle, int horizontal_direction, int vertical_direction) {
	if (ball_x0 >= SCREEN_W)	
	{
		horizontal_direction = 2;
		if (vertical_direction == 1 || vertical_direction == 0)
		{
			vertical_direction = 1;			
		}
		else vertical_direction = 2;
	}

			if (ball_x0 <= 0)
			{
				horizontal_direction = 1;
				if (vertical_direction == 2 || vertical_direction == 0)
				{
					vertical_direction = 2;
				}
				else
					vertical_direction = 1;
			}

			if (ball_y0 <= 0)
			{
				//horizontal_direction = 1;
				vertical_direction = 2;
				yangle = rand() % 4 + 1;

			}
			if (ball_y0 >= SCREEN_H)
			{
				//horizontal_direction = 1;
				vertical_direction = 1;
				yangle = rand() % 4 + 1;
			}
		}
void padhit(Players player[], int vertical_direction, int horizontal_direction, float ball_y0, float ball_x0){

			// BLOCK BALL RIGHT PADDLE

			if (ball_x0 < player[1].xPos - 10 && ball_x0 > player[1].xPos - 16 && ball_y0 >= player[1].yPos - 45 && ball_y0 <= player[1].yPos + 145)
			{

				horizontal_direction = 2;
				if (vertical_direction == 1 || vertical_direction == 0)
				{
					vertical_direction = 1;
				}
				else
					vertical_direction = 2;

			}

			// BLOCK BALL LEFT PADDLE

			if (player[0].xPos + 10 < ball_x0 && player[0].xPos + 16 > ball_x0 && player[0].yPos - 45 < ball_y0 && player[0].yPos + 145 > ball_y0)
			{

				horizontal_direction = 1;
				if (vertical_direction == 1 || vertical_direction == 0)
				{
					vertical_direction = 1;
				}
				else
					vertical_direction = 2;

			}

		}
#endif // !client_h
