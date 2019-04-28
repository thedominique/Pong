#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL_net.h"
#include "SDL.h"
#include "rendering.h"
#include "objects.h"
#include "server_handler.h"
#include "event_handler.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "display_text.h"
#include "create_texture.h"
#include "init_master.h"


int main(int argc, char **argv) {
	SDL_Init(SDL_INIT_EVERYTHING);
	SDLNet_Init();
	UDPsocket client_socket;
	IPaddress ipaddress;
	UDPpacket *packet_send;
	UDPpacket *packet_receive;

	
	int choice = menu();
	if (choice == 2) {
		return 0;
	}
	

	GameState gamestate;
	Paddle mypaddle;
	OldLives oldLives;
	initPlayers(&gamestate, &oldLives, &mypaddle);
	
	
	float NET_TICK_RATE = 60;

	/* Open a socket on random port */
	client_socket = SDLNet_UDP_Open(2317);

	/* Resolve server name  */
	SDLNet_ResolveHost(&ipaddress, "localhost", 1234);

	/* Allocate memory for the packet */
	packet_send = SDLNet_AllocPacket(512);
	packet_receive = SDLNet_AllocPacket(512);

	SDL_Window *window;
	SDL_Renderer *renderer;

	//Create an application window with the following settings:
	window = SDL_CreateWindow("Game Window",                     // window title
		SDL_WINDOWPOS_UNDEFINED,           // initial x position
		SDL_WINDOWPOS_UNDEFINED,           // initial y position
		WINDOW_WIDTH,                      // width, in pixels
		WINDOW_HEIGHT,                     // height, in pixels
		0                                  // flags
	);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	//HANTERAR BILD (BORDET,SPELARE)
	IMG_Init(IMG_INIT_JPG);

	//BOARD AND PLAYERS TEXTURES
	SDL_Texture *board = init_board_texture(renderer);
	SDL_Texture *player1Texture = init_player1_texture(renderer);
	SDL_Texture *player2Texture = init_player2_texture(renderer);
	SDL_Texture *player3Texture = init_player3_texture(renderer);
	
	//TEXT TEXTURES
	SDL_Texture *text = init_text(&gamestate, renderer);

	int done = 0;
	int i = 0;
	long ticks_per_sec = SDL_GetPerformanceFrequency();
	long tick_t0 = SDL_GetPerformanceCounter();
	long next_net_tick = tick_t0;
	long net_tick_interval = (1 / NET_TICK_RATE) * ticks_per_sec;

	while (!done) {
		long tick_t1 = SDL_GetPerformanceCounter();
		double dt = (tick_t1 - tick_t0) / (double)ticks_per_sec;

		if (SDLNet_UDP_Recv(client_socket, packet_receive)) {
			Receive *receive = (Receive *)packet_receive->data;
			//printf("RECEIVED: %lf\n", receive->b1.x);
			//printf("gamestate player[0].x: %lf\n", gamestate.players[0].x);
			receive_server_values(packet_receive, &gamestate);
		}

		//update texten OM det har skett någon skillnad
		if (compare_lives(&gamestate, &oldLives)) {
			printf("YES");

			text = update_text(&gamestate, renderer, &oldLives);
			
		}

		//Render display
		doRender(renderer, &gamestate, &mypaddle, board, text, player1Texture,player2Texture, player3Texture);

		done = processEvents(window, &mypaddle, dt, &gamestate);
		if (tick_t1 >= next_net_tick) {
			for (i = 0; i < 4; i++) {
				packet_send->data = (void*)&mypaddle;
				packet_send->channel = -1;
				packet_send->len = sizeof(mypaddle);
				packet_send->maxlen = packet_send->len + 20;
				packet_send->address = ipaddress;

				if (SDLNet_UDP_Send(client_socket, -1, packet_send) == 0) {
					printf("failed to send from client/n");
				}
			}
			next_net_tick += net_tick_interval;
		}
		//RESET KEYS TILL NÄSTA LOOP OMGÅNG
		reset_keys(&mypaddle);
	}
	SDLNet_FreePacket(packet_send);
	SDLNet_FreePacket(packet_receive);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyTexture(text);
	SDL_DestroyTexture(board);
	SDL_DestroyTexture(player1Texture);
	SDL_DestroyTexture(player2Texture);
	SDL_DestroyTexture(player2Texture);
	SDL_DestroyWindow(window);
	IMG_Quit();

	SDLNet_Quit();

	return EXIT_SUCCESS;
}
