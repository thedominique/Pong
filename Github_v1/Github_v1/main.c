#include <stdio.h>
#include <SDL.h>
#include <SDL_net.h>
#include <string.h>
#include <stdbool.h>
#include "game_state.h"
#include "constants.h"
//#include "render.h"
#include "client.h"

bool address_equal(IPaddress a, IPaddress b);
void print_ip(int ip);	// bara för att identifiera att rätt maskin hittad
void openClientState(SDL_Renderer* renderer, UDPsocket * clientsock, IPaddress server_addr, SDL_Window* window);
void openServerState(UDPsocket* serversock);

int main(int argc, char** argv[]) {
	IPaddress ipaddress;
	UDPsocket socket;
	SDL_Renderer* render;
	SDL_Window* window = 0;

	int hesitation;
	char answer[3];
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("SDL_INIT_EVERYTHING: %s", SDL_GetError());
		exit(2);
	}
	SDLNet_Init();

	do {
		printf("Welcome to PONG\nDo you want to start as a Client or Server?\n");
		printf("Client = 1 Server = 2\n");
		printf("Exit = 0\n");
		gets(answer);
		hesitation = atoi(answer);
		if (hesitation == 1)
		{
			printf("Initiate as client...\n");
			SDL_Delay(1000);
			// INSERT CLIENT FUNCTION
			//render = init_Window(window);
			clientState();
			//openClientState();jku
		}
		else if (hesitation == 2)
		{
			printf("Initiate as server...\n");
			SDL_Delay(1000);
			// INSERT SERVER FUNCTION
			SDLNet_ResolveHost(&ipaddress, NULL, LOCAL_PORT);
			socket = SDLNet_UDP_Open(LOCAL_PORT);
			if (!socket)
			{
				printf("SDLNet_UDP_Open: %s\n", SDL_GetError());
				exit(2);
			}
			openServerState(&socket);
		}
		
	} while (hesitation != 0);

	//game_Loop(&serversock, render, window);

	SDLNet_UDP_Close(&socket);
	SDLNet_Quit();
	SDL_Quit();

	return 0;
}

void print_ip(int ip) {

	unsigned char bytes[4];
	bytes[0] = ip & 0xFF;
	bytes[1] = (ip >> 8) & 0xFF;
	bytes[2] = (ip >> 16) & 0xFF;
	bytes[3] = (ip >> 24) & 0xFF;
	printf("%d.%d.%d.%d\n", bytes[3], bytes[2], bytes[1], bytes[0]);
}

/*void openClientState(SDL_Renderer* renderer, UDPsocket * clientsock, IPaddress server_addr, SDL_Window* window){
	int which_player = 0;
	int quit;
	//INIT PLAYER POSITION
	float p1_posX0 = 0, p1_posY0 = PAD_INIT_Y0;
	float p2_posX0 = SCREEN_W, p2_posY0 = PAD_INIT_Y0;
	float p_posX1 = 0, p_posY1 = 0; // Updates player position
	float  p_posY = 0, p_posX = 0;	// Takes data from packet. Updates X1 position.

	//BALL POSITION
	float ball_x0 = BALL_INIT_X0, ball_y0 = BALL_INIT_Y0;
	float ball_X1, ball_Y1;

	SDL_Event event;
	SDL_bool buttonPress = SDL_FALSE;
	UDPpacket* packet;
	Players player[3] = { 0,0,0 };

	long ticks_per_sec = SDL_GetPerformanceFrequency();
	long tick_t0 = SDL_GetPerformanceCounter();

	long next_net_tick = tick_t0;
	long net_tick_interval = (1 / NET_TICK_RATE) * ticks_per_sec;

	//Allocate memory for the packet 
	packet = SDLNet_AllocPacket(1024);
	// Main loop
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

		//RECIEVE PACKET
		if (SDLNet_UDP_Recv((*clientsock), packet))
		{
			server_packet_t* server_packet = (server_packet_t*)packet->data;
			p_posY1 = server_packet->p_yPos;
			p_posX1 = server_packet->p_xPos;
			which_player = server_packet->which_player;	// UNPACK VALUE TO CORRESPONDING
														// IPADDRESS WHICH THE SERVER SORTS OUT

			printf("Position y: %f", p_posY1);
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
			ball_X1 = server_packet->ball_xPos;
			ball_Y1 = server_packet->ball_yPos;
		}

		//SDL_STATES
		const Uint8* state = SDL_GetKeyboardState(NULL);
		if (state[SDL_SCANCODE_W] && p_posY > 0)
		{
			p_posY -= PAD_SPEED_Y * dt;

		}
		if (state[SDL_SCANCODE_S] && p_posY < SCREEN_H - PAD_HEIGHT)
		{
			p_posY += PAD_SPEED_Y * dt;

		}
		if (state[SDL_SCANCODE_A] && p_posX > 0)
		{
			p_posX -= PAD_SPEED_X * dt;

		}
		if (state[SDL_SCANCODE_D] && p_posX < SCREEN_W - PAD_WIDTH)
		{
			p_posX += PAD_SPEED_X * dt;
		}

		//SEND PACKAGE
		if (tick_t1 >= next_net_tick)
		{
			server_packet_t server_packet;
			server_packet.p_yPos = p_posY;
			server_packet.p_xPos = p_posX;

			server_packet.which_player = which_player;

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

		//RENDERER
				//(renderer, p1_posX0, p1_posY0, p2_posX0, p2_posY0, ball_x0, ball_y0, player);
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
}*/

void openServerState(UDPsocket* serversock){
	IPaddress client_addr[3] = { 0, 0, 0 };
	SDL_Event event;
	int which_player = 0;
	int i = 0, new_client = 0;
	float p_xPos = 0, p_yPos = 0;
	float ball_xPos, ball_yPos;
	//float ballSpeed = BALL_SPEED; 
	float PADDLE_INIT_Y0 = SCREEN_H / 2;

	//float paddleSpeed = PLAYER_SPEED;
	gamestate_t gamestate;
	Players_t player[2] = { 0,0 };
	UDPpacket *packet;
	/*long* ticks_per_sec = 0;
	long* tick_t0 = 0;
	long* next_net_tick = 0;
	long* net_tick_interval = 0;*/

	long ticks_per_sec = SDL_GetPerformanceFrequency();
	long tick_t0 = SDL_GetPerformanceCounter();

	long next_net_tick = tick_t0;
	long net_tick_interval = (1 / NET_TICK_RATE) * ticks_per_sec;

	packet = SDLNet_AllocPacket(1024);

	p_yPos = PADDLE_INIT_Y0;
	while (1)
	{
		long  tick_t1 = SDL_GetPerformanceCounter();
		void init_tick();
		double dt = (tick_t1 - tick_t0) / (double)(ticks_per_sec);

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
			p_yPos += 1; //client_packet->p_yPos;
			p_xPos = client_packet->p_xPos;
			ball_xPos = client_packet->ball_xPos;
			ball_yPos = client_packet->ball_yPos;
			//which_player = player->which_player;
		}

		if (tick_t1 >= next_net_tick)
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
				next_net_tick += net_tick_interval;
			}
		}
		//windowRendering(render, &gamestate, player);
		tick_t0 = tick_t1;
		SDLNet_FreePacket(packet);
	}
}

bool address_equal(IPaddress a, IPaddress b) {
	return a.host == b.host && a.port == b.port;
}
