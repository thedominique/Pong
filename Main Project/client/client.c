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
#include "soundeffect.h"




int playagain_or_exit(SDL_Renderer *renderer, Paddle *mypaddle, SDL_Texture *play_text, SDL_Texture *quit_text, SDL_Texture *lobby_text, SDL_Texture *waiting_text, SDL_Texture *lobby_backgroundTexture)
{
	int quit = 0;
	SDL_Event event; 
	//background color
	SDL_SetRenderDrawColor(renderer, 0, 0, 30, 255);
	SDL_RenderClear(renderer);

	SDL_Rect lobby_backgroundBox = { 0, 0,WINDOW_WIDTH,WINDOW_HEIGHT };
	SDL_RenderCopy(renderer, lobby_backgroundTexture, NULL, &lobby_backgroundBox);

	if (mypaddle->playAgain)
	{
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	}
	else
	{
		SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
	}

	SDL_Rect playagainBox = { WINDOW_WIDTH / 2 - 50 - TEXT_BOX_WIDTH / 4, WINDOW_HEIGHT / 2 - 50 - 100, TEXT_BOX_WIDTH, TEXT_BOX_HEIGHT };
	SDL_RenderFillRect(renderer, &playagainBox);
	
	SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
	SDL_Rect quitBox = { WINDOW_WIDTH / 2 - 50 - TEXT_BOX_WIDTH / 4, WINDOW_HEIGHT / 2 + 60 - 100 +50, TEXT_BOX_WIDTH, TEXT_BOX_HEIGHT };
	SDL_RenderFillRect(renderer, &quitBox);


	// PLAY TEXT
	SDL_Rect play_textRect = { WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT / 2 - 50 - 75 ,150 ,50 };
	SDL_RenderCopy(renderer, play_text, NULL, &play_textRect);

	// QUIT TEXT
	SDL_Rect quit_textRect = { WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT / 2  + 30,150,50 };
	SDL_RenderCopy(renderer, quit_text, NULL, &quit_textRect);

	// LOBBBY TEXT
	SDL_Rect lobby_textRect = { WINDOW_WIDTH / 2 -175 , 0, 400, 50 };
	SDL_RenderCopy(renderer, lobby_text, NULL, &lobby_textRect);

	// WAITING TEXT
	SDL_Rect waiting_textRect = { WINDOW_WIDTH / 2 - 175 , 100, 400, 50 };
	SDL_RenderCopy(renderer, waiting_text, NULL, &waiting_textRect);


	SDL_RenderPresent(renderer);


	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT ||
			(event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE) ||
			(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
		{
			mypaddle->quit = 1;
		}
		if (event.type == SDL_MOUSEBUTTONDOWN &&
			event.button.x >= playagainBox.x &&
			event.button.x <= (playagainBox.x + playagainBox.w) &&
			event.button.y >= playagainBox.y &&
			event.button.y <= (playagainBox.h + playagainBox.y))
		{
			mypaddle->playAgain= 1;
			mypaddle->gameStarted = 1;
		}

		if (event.type == SDL_MOUSEBUTTONDOWN &&
			event.button.x >= quitBox.x &&
			event.button.x <= (quitBox.x + quitBox.w) &&
			event.button.y >= quitBox.y &&
			event.button.y <= (quitBox.h + quitBox.y))
		{
			mypaddle->quit = 1;
		}
	}
}


int main(int argc, char **argv) {
	SDL_Init(SDL_INIT_EVERYTHING);
	SDLNet_Init();
	UDPsocket client_socket;
	IPaddress ipaddress;
	UDPpacket *packet_send;
	UDPpacket *packet_receive;




	GameState gamestate;
	Paddle mypaddle;
	OldLives oldLives;
	//SOUND
	Mix_Init(MIX_INIT_MP3);
	sound soundeffect;
	initsound(&soundeffect);
	initPlayers(&gamestate, &oldLives, &mypaddle);
	oldLives.angel = 0.5;

	float NET_TICK_RATE = 60;

	/* Open a socket on random port */
	client_socket = SDLNet_UDP_Open(5367);

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

	
	int choice = menu(renderer);
	if (choice == 2)
	{
		return 0;
	}
	
	
	//BOARD AND PLAYERS TEXTURES
	SDL_Texture *board = init_board_texture(renderer);
	SDL_Texture *player1Texture = create_player_texture(renderer, "player1.jpg");
	SDL_Texture *player2Texture = create_player_texture(renderer, "player2.jpg");
	SDL_Texture *player3Texture = create_player_texture(renderer, "player3.jpg");
	SDL_Texture *backgroundTexture = init_background(renderer);
	SDL_Texture *lobby_backgroundTexture = create_player_texture(renderer, "lobbybackground.jpg");

	//TEXT TEXTURES
	SDL_Texture *text = init_text(&gamestate, renderer);
	SDL_Texture *play_text = create_text(&gamestate, renderer, "Ready up", 100);
	SDL_Texture *quit_text = create_text(&gamestate, renderer, "Quit", 100);
	SDL_Texture *lobby_text = create_text(&gamestate, renderer, "Lobby", 100);
	SDL_Texture *waiting_text = create_text(&gamestate, renderer, "Waiting for players...", 100);
	
	//SOUND 


	int done = 0;
	int i = 0;
	long ticks_per_sec = SDL_GetPerformanceFrequency();
	long tick_t0 = SDL_GetPerformanceCounter();
	long next_net_tick = tick_t0;
	long net_tick_interval = (1 / NET_TICK_RATE) * ticks_per_sec;

	while (!done) {
		long tick_t1 = SDL_GetPerformanceCounter();
		double dt = (tick_t1 - tick_t0) / (double)ticks_per_sec;

		if (SDLNet_UDP_Recv(client_socket, packet_receive)) 
		{
			Receive *receive = (Receive *)packet_receive->data;
			//printf("RECEIVED: %lf\n", receive->b1.x);
			//printf("gamestate player[0].x: %lf\n", gamestate.players[0].x);
			receive_server_values(packet_receive, &gamestate,&mypaddle);
			play_soundeffect(&soundeffect, &gamestate);
		}

		//update texten OM det har skett någon skillnad
		if (compare_lives(&gamestate, &oldLives)) 
		{
			text = update_text(&gamestate, renderer, &oldLives);
		}
		//printf("game over: %d\n", gamestate.gameover);
		//Render display
		//printf("playagain: %d\n", mypaddle.playAgain);
		
		if (!gamestate.gameover && gamestate.gameStarted)
		{
			mypaddle.playAgain = 0;
			doRender(renderer, &gamestate, &mypaddle, board, text, player1Texture, player2Texture, player3Texture, backgroundTexture, &oldLives);
		}
		else if (gamestate.gameover || !gamestate.gameStarted)
		{
			playagain_or_exit(renderer,&mypaddle, play_text, quit_text, lobby_text, waiting_text, lobby_backgroundTexture);
		}
		

		//printf("ball collision: %d\n", gamestate.ball.collision);
		//printf("play again: %d", mypaddle.playAgain);
		done = processEvents(window, &mypaddle, dt, &gamestate);
		if (tick_t1 >= next_net_tick)
		{
			for (i = 0; i < 4; i++) 
			{
				packet_send->data = (void*)&mypaddle;
				packet_send->channel = -1;
				packet_send->len = sizeof(mypaddle);
				packet_send->maxlen = packet_send->len + 20;
				packet_send->address = ipaddress;

				if (SDLNet_UDP_Send(client_socket, -1, packet_send) == 0)
				{
					printf("failed to send from client/n");
				}
			}

			next_net_tick += net_tick_interval;

			if (mypaddle.quit)
			{
				break;
			}

		}
		//RESET KEYS TILL NÄSTA LOOP OMGÅNG
		reset_keys(&mypaddle);

	}
	SDLNet_FreePacket(packet_send);
	SDLNet_FreePacket(packet_receive);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyTexture(text);
	SDL_DestroyTexture(board);
	SDL_DestroyTexture(backgroundTexture);
	SDL_DestroyTexture(player1Texture);
	SDL_DestroyTexture(player2Texture);
	SDL_DestroyTexture(player2Texture);
	SDL_DestroyTexture(play_text);
	SDL_DestroyTexture(quit_text);
	SDL_DestroyTexture(lobby_text);
	SDL_DestroyTexture(waiting_text);
	SDL_DestroyWindow(window);
	//Mix_FreeMusic(soundeffect.soundeffect);
	IMG_Quit();
	SDLNet_Quit();

	return EXIT_SUCCESS;
}
