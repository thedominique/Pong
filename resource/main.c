//
//  main.c
//  ClientEx
//
//  Created by Dominique Persson on 2019-03-28.
//  Copyright © 2019 Dominique Persson. All rights reserved.
//

#include <stdio.h>
#include <SDL.h>

#include <SDL_net.h>
#include <string.h>
#include <stdbool.h>

#define BALL_WIDTH 0.015
#define	BALL_HEIGHT 0.02
//#define 

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

const float PAD_WIDTH = 0.02;
const float PAD_HEIGHT = 0.15;
const float PAD_PAD = 0.02;
const float PAD_SPEED_PER_SEC = 1;

//float BALL_WIDTH = 0.015;
//float BALL_HEIGHT = 0.02;
float BALL_SPEED_PER_SEC = 0.4;

float BALL_INIT_X0 = 0.5 - BALL_WIDTH / 2;
float BALL_INIT_Y0 = 0.5 - BALL_HEIGHT / 2;

int SCREEN_WIDTH = 1000;
int SCREEN_HEIGHT = 500;

int SERVER_PORT = 6666;

float NET_TICK_RATE = 20;

typedef struct client_packet {
	double pad_y;
} client_packet_t;

typedef struct server_packet {
	double pad_y;
	double ball_x;
	double ball_y;
	double ball_dv_x;
	double ball_dv_y;
	bool is_player1;
} server_packet_t;

bool address_equal(IPaddress a, IPaddress b) {
	return a.host == b.host && a.port == b.port;
}

void main_loop(SDL_Renderer* renderer, UDPsocket* udpsock, IPaddress server_addr, bool is_server) {
	double pad1_y = 0, pad2_y = 0;
	double ball_x = BALL_INIT_X0, ball_y = BALL_INIT_Y0;
	double ball_dv_x = BALL_SPEED_PER_SEC, ball_dv_y = BALL_SPEED_PER_SEC;

	IPaddress client_addr[2] = {0};		// What?

	bool is_player1 = true;

	bool move_up = false;
	bool move_dn = false;

	long ticks_per_sec = SDL_GetPerformanceFrequency();
	long tick_t0 = SDL_GetPerformanceCounter();

	long next_net_tick = tick_t0;
	long net_tick_interval = (1 / NET_TICK_RATE) * ticks_per_sec;

	for (;;) {
		long tick_t1 = SDL_GetPerformanceCounter();
		double dt = (tick_t1 - tick_t0) / (double)ticks_per_sec;

		SDL_Event e;
		if (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				switch (e.key.keysym.sym) {
				case SDLK_UP:
					move_up = (e.type == SDL_KEYDOWN);
					break;
				case SDLK_DOWN:
					move_dn = (e.type == SDL_KEYDOWN);
					break;
				default:
					break;
				}
				break;
			case SDL_QUIT:
				return;
			default:
				break;
			}
			if (e.type == SDL_QUIT) {
				break;
			}
		}

		UDPpacket packet;
		char buf[1600];
		packet.data = (void*)buf;
		packet.maxlen = sizeof(buf);
		int numrecv = SDLNet_UDP_Recv(udpsock, &packet);
		if (numrecv > 0) {
			if (is_server) {
				bool is_client_player1;
				if (address_equal(packet.address, client_addr[0])) {
					is_client_player1 = true;
				}
				else if (address_equal(packet.address, client_addr[1])) {
					is_client_player1 = false;
				}
				else {
					is_client_player1 = is_player1;
					client_addr[is_client_player1 ? 0 : 1] = packet.address;
					is_player1 = !is_player1;
				}
				client_packet_t* client_packet = (client_packet_t*)packet.data;

				double* pad_y = is_client_player1 ? &pad1_y : &pad2_y;
				*pad_y = client_packet->pad_y;
				//printf("received: %f %d\n", *pad_y, client_pack);
			}
			else {
				server_packet_t* server_packet = (server_packet_t*)packet.data;

				double* pad_y = is_player1 ? &pad2_y : &pad1_y;
				*pad_y = server_packet->pad_y;

				ball_x = server_packet->ball_x;
				ball_y = server_packet->ball_y;
				ball_dv_x = server_packet->ball_dv_x;
				ball_dv_y = server_packet->ball_dv_y;
				is_player1 = server_packet->is_player1;
			}
		}

		if (!is_server) {
			double* pad_y = is_player1 ? &pad1_y : &pad2_y;
			if (move_up != move_dn) {
				*pad_y += (move_up ? -PAD_SPEED_PER_SEC : PAD_SPEED_PER_SEC) * dt;
				*pad_y = MIN(1 - PAD_HEIGHT, *pad_y);
				*pad_y = MAX(0, *pad_y);
			}
		}

		if (tick_t1 >= next_net_tick) {
			if (is_server) {
				server_packet_t server_packet;// = {};	
				server_packet.ball_x = ball_x;
				server_packet.ball_y = ball_y;
				server_packet.ball_dv_x = ball_dv_x;
				server_packet.ball_dv_y = ball_dv_y;
				for (int i = 0; i < 2; i++) {
					if (client_addr[i].port == 0) {
						continue;
					}
					double* pad_y = (i == 1) ? &pad1_y : &pad2_y;
					server_packet.pad_y = *pad_y;
					server_packet.is_player1 = (i == 0);
					UDPpacket packet;
					packet.address = client_addr[i];
					packet.channel = -1;
					packet.data = (void*)&server_packet;
					packet.len = sizeof(server_packet);
					packet.maxlen = packet.len;

					if (SDLNet_UDP_Send(udpsock, -1, &packet) == 0) {
						printf("SDLNet_UDP_Send: failed %s\n", SDLNet_GetError());
					}
				}
			}
			else {
				client_packet_t client_packet;// = {NULL};	// what?
				double* pad_y = is_player1 ? &pad1_y : &pad2_y;
				//printf("sending: %f\n", *pad_y);
				client_packet.pad_y = *pad_y;
				UDPpacket packet;
				packet.address = server_addr;
				packet.channel = -1;
				packet.data = (void*)&client_packet;
				packet.len = sizeof(client_packet);
				packet.maxlen = packet.len;
				if (SDLNet_UDP_Send(udpsock, -1, &packet) == 0) {
					printf("SDLNet_UDP_Send: failed %s\n", SDLNet_GetError());
				}
			}
			next_net_tick += net_tick_interval;
		}

		/*
		pad_y = !is_player1 ? &pad1_y : &pad2_y;
		if (move_up != move_dn) {
			*pad_y += (move_up? -PAD_SPEED_PER_SEC: PAD_SPEED_PER_SEC) * dt;
			*pad_y = MIN(1 - PAD_HEIGHT, *pad_y);
			*pad_y = MAX(0, *pad_y);
		}
		*/

		ball_x += ball_dv_x * dt;
		ball_y += ball_dv_y * dt;

		// Ball collision.
		if (ball_y <= 0) {
			ball_dv_y = BALL_SPEED_PER_SEC;
		}
		if (ball_y >= 1 - BALL_HEIGHT) {
			ball_dv_y = -BALL_SPEED_PER_SEC;
		}
		if (ball_x <= 0 || ball_x - BALL_WIDTH >= 1) {
			ball_x = BALL_INIT_X0;
			ball_y = BALL_INIT_Y0;
			ball_dv_x = BALL_SPEED_PER_SEC;
			ball_dv_y = BALL_SPEED_PER_SEC;
		}

		if (ball_y + BALL_HEIGHT > pad1_y && ball_y < pad1_y + PAD_HEIGHT) {
			if (ball_x + BALL_WIDTH > PAD_PAD && ball_x < PAD_PAD + PAD_WIDTH) {
				ball_dv_x = BALL_SPEED_PER_SEC;
			}
		}

		if (ball_y + BALL_HEIGHT > pad2_y && ball_y < pad2_y + PAD_HEIGHT) {
			if (ball_x + BALL_WIDTH > 1 - PAD_PAD - PAD_WIDTH && ball_x < 1 - PAD_PAD) {
				ball_dv_x = -BALL_SPEED_PER_SEC;
			}
		}

		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

		// Draw pad 1.
		SDL_Rect fillRect = { PAD_PAD * SCREEN_WIDTH, pad1_y*SCREEN_HEIGHT, PAD_WIDTH * SCREEN_WIDTH, PAD_HEIGHT * SCREEN_HEIGHT };
		SDL_RenderFillRect(renderer, &fillRect);

		// Draw pad 2.
		SDL_Rect fillRect2 = { (1.0 - PAD_PAD - PAD_WIDTH) * SCREEN_WIDTH, pad2_y*SCREEN_HEIGHT, PAD_WIDTH * SCREEN_WIDTH, PAD_HEIGHT * SCREEN_HEIGHT };
		SDL_RenderFillRect(renderer, &fillRect2);

		// Draw ball.
		SDL_Rect fillRect3 = { ball_x * SCREEN_WIDTH, ball_y*SCREEN_HEIGHT, BALL_WIDTH * SCREEN_WIDTH, BALL_HEIGHT * SCREEN_HEIGHT };
		SDL_RenderFillRect(renderer, &fillRect3);

		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderPresent(renderer);

		tick_t0 = tick_t1;
	}
}

int main(int argc, const char * argv[]) {
	//argc = 2;
	IPaddress server_addr;
	int local_port = 0;
	bool is_server = false;
	UDPsocket udpsock;
	SDLNet_Init(); // Windows

	SDL_Init(SDL_INIT_EVERYTHING);

	if (argc > 1) {
		printf("I'm a client!\n");
		if (SDLNet_ResolveHost(&server_addr, argv[1], local_port) != 0) {
			printf("SDLNet_ResolveHost: failed %s\n", SDLNet_GetError());
			exit(2);
		}
	}
	else {
		printf("I'm a server!\n");
		local_port = SERVER_PORT;
		is_server = true;
		SDLNet_ResolveHost(&server_addr, NULL, local_port);
	}

	udpsock = SDLNet_UDP_Open(local_port);
	if (!udpsock) {
		printf("SDLNet_UDP_Open: %s\n", SDLNet_GetError());
		exit(2);
	}

	SDL_Window* window = SDL_CreateWindow(
		"Ultra pong",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
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

	main_loop(renderer, udpsock, server_addr, is_server);


	/*
	SDL_Delay(100000);  // Pause execution for 3000 milliseconds, for example



	SDLNet_Init();

	IPaddress ip;
	//char adress[]="www.google.com";
	SDLNet_ResolveHost(&ip, "130.229.156.68", 1234); // server ip adress
	//130.229.149.121
	//130.229.156.68

	// tcp connection
	const char* sendtext = "Jag vill ha en cheese burgare\n";
	TCPsocket client = SDLNet_TCP_Open(&ip);
	//TCPsocket server = SDLNet_TCP_Open(&remote_ip);

	//IPaddress *remote_ip;

	char text[100];
	SDLNet_TCP_Recv(client, text, 100);
	//remote_ip = SDLNet_TCP_GetPeerAddress(server);
	SDLNet_TCP_Send(client, sendtext, strlen(sendtext));
	printf("%s",text);


	SDLNet_TCP_Close(client);
	*/
	SDLNet_Quit();
	SDL_Quit();

}