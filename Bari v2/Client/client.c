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








int main(int argc, char **argv)
{

	SDL_Init(SDL_INIT_EVERYTHING);
	SDLNet_Init();
	UDPsocket client_socket;
	IPaddress ipaddress;
	UDPpacket *packet_send;
	UDPpacket *packet_receive;
	


	GameState gamestate;

	gamestate.players[0].x = 0;
	gamestate.players[0].y = WINDOW_HEIGHT / 2;
	gamestate.players[0].w = PADDLE_WIDTH;
	gamestate.players[0].h = PADDLE_HEIGHT;

	gamestate.players[1].x = WINDOW_WIDTH - PADDLE_WIDTH;
	gamestate.players[1].y = WINDOW_HEIGHT / 2;
	gamestate.players[1].w = PADDLE_WIDTH;
	gamestate.players[1].h = PADDLE_HEIGHT;

	gamestate.ball.x = WINDOW_WIDTH / 2;
	gamestate.ball.y = WINDOW_HEIGHT / 2;
	gamestate.ball.h = BALL_SIZE;
	gamestate.ball.w = BALL_SIZE;
	gamestate.ball.xVel = BALL_SPEED;
	gamestate.ball.yVel = BALL_SPEED;

	float NET_TICK_RATE = 30;

	/* Open a socket on random port */
	client_socket = SDLNet_UDP_Open(2344);

	/* Resolve server name  */
	SDLNet_ResolveHost(&ipaddress, "192.168.0.45", 1234);


	/* Allocate memory for the packet */
	packet_send = SDLNet_AllocPacket(512);
	packet_receive = SDLNet_AllocPacket(512);
	//printf("Fill the buffer\n>");
	//scanf("%s", (char *)packet->data);
	//packet_send->len = strlen((char *)packet_send->data) + 1;
	//packet_receive->len = strlen((char *)packet_send->data) + 1;

	SDL_Window *window;
	SDL_Renderer *renderer;
	Paddle mypaddle;
	mypaddle.x = 30;
	mypaddle.y = 30;
	mypaddle.h = PADDLE_HEIGHT;
	mypaddle.w = PADDLE_WIDTH;

	//Create an application window with the following settings:
	window = SDL_CreateWindow("Game Window",                     // window title
		SDL_WINDOWPOS_UNDEFINED,           // initial x position
		SDL_WINDOWPOS_UNDEFINED,           // initial y position
		WINDOW_WIDTH,                      // width, in pixels
		WINDOW_HEIGHT,                     // height, in pixels
		0                                  // flags
	);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	
	IMG_Init(IMG_INIT_JPG);
	SDL_Texture *board;
	SDL_Surface *boardSurface = NULL;
	boardSurface = IMG_Load("test.jpg");
	if (boardSurface == NULL)
	{
		printf("Could not find image! \n");
		SDL_Quit();
		return 1;
	}
	board = SDL_CreateTextureFromSurface(renderer, boardSurface);
	SDL_FreeSurface(boardSurface);




	int done=0;
	int i = 0;
	long ticks_per_sec = SDL_GetPerformanceFrequency();
	long tick_t0 = SDL_GetPerformanceCounter();
	long next_net_tick = tick_t0;
	long net_tick_interval = (1 / NET_TICK_RATE) * ticks_per_sec;

	while (!done)
	{
		long tick_t1 = SDL_GetPerformanceCounter();
		double dt = (tick_t1 - tick_t0) / (double)ticks_per_sec;


		if (SDLNet_UDP_Recv(client_socket, packet_receive))
		{
			Receive *receive = (Receive *)packet_receive->data;
			printf("RECEIVED: %lf\n", receive->b1.x);
			printf("gamestate ball.x: %lf\n", gamestate.ball.x);
			//doRender(renderer, &left_paddle, &right_paddle);
			receive_server_values(packet_receive, &gamestate);
		}
		//Render display
		doRender(renderer, &gamestate, &mypaddle, board);


		printf("gamestate ball.x: %lf\n", gamestate.ball.x);
		printf("my paddle: x %lf\n", mypaddle.x);





		done = processEvents(window, &mypaddle, dt);

		if (tick_t1 >= next_net_tick)
		{
			for (i = 0; i < 4; i++)
			{

				packet_send->data = (void*)&mypaddle;
				packet_send->channel = -1;
				packet_send->len = sizeof(mypaddle);
				packet_send->maxlen = packet_send->len+10;
				packet_send->address = ipaddress;
				//printf("TRIED TO SEND: %d\n", gamestate.ball.x);


				if (SDLNet_UDP_Send(client_socket, -1, packet_send) == 0)
				{
					printf("failed to send from server/n");
				}

			}

			next_net_tick += net_tick_interval;
		}
		


		
	}

	SDL_DestroyTexture(board);
	SDLNet_FreePacket(packet_send);
	SDLNet_FreePacket(packet_receive);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDLNet_Quit();

	return EXIT_SUCCESS;
}