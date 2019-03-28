
//
//  main.c
//  TCP_serverEx
//
//  Created by Dominique Persson on 2019-03-28.
//  Copyright © 2019 Dominique Persson. All rights reserved.
//

#include <stdio.h>
#include <SDL.h>
#include <SDL_net.h>
#include <string.h>

int main(int argc, const char * argv[]) {
	// insert code here...
	printf("Hello");
	SDL_Init(SDL_INIT_EVERYTHING);
	SDLNet_Init();

	IPaddress ip;
	//char adress[]="www.google.com";
	SDLNet_ResolveHost(&ip, NULL, 1234); // server ip adress

	// tcp connection
	TCPsocket server = SDLNet_TCP_Open(&ip);
	TCPsocket client;
	TCPsocket client2;

	//const char* text = "SMF Ricardo";
	//char sendtext[100];

	//Ball start position
	int* ball_posx = 320;
	int* ball_posy = 240;

	//Right flipper start position
	int* client_posx = 740;
	int* client_posy = 240;
	//Left flipper start position
	int* client2_posx = 40;
	int* client2_posy = 240;

	while (1) {
		client = SDLNet_TCP_Accept(server);
		
		while (client) {
			// communicate w client
			
			SDLNet_TCP_Recv(client, client_posx, sizeof(client_posx));
			
			SDLNet_TCP_Recv(client, client_posy, sizeof(client_posy));

			SDLNet_TCP_Recv(client, ball_posx, sizeof(ball_posx));

			SDLNet_TCP_Recv(client, ball_posy, sizeof(ball_posy));
			
			

			/*SDLNet_TCP_Send(client, client2_posx, sizeof(client2_posx));
			SDLNet_TCP_Send(client2, client_posx, sizeof(client_posx));
			SDLNet_TCP_Send(client, client2_posy, sizeof(client2_posy));
			SDLNet_TCP_Send(client2, client_posy, sizeof(client_posy));*/
			SDL_Delay(1);


			printf("hey");

			SDLNet_TCP_Close(client);
			break;
		}
		client2 = SDLNet_TCP_Accept(server);
		while (client2) {

			SDLNet_TCP_Send(client2, client_posx, sizeof(client_posx));
			SDLNet_TCP_Send(client2, client_posy, sizeof(client_posy));

			SDLNet_TCP_Send(client2, ball_posx, sizeof(ball_posx));

			SDLNet_TCP_Send(client2, ball_posy, sizeof(ball_posy));

			SDLNet_TCP_Recv(client2, client2_posx, sizeof(client2_posx));
			
			SDLNet_TCP_Recv(client2, client2_posy, sizeof(client2_posy));



			printf("hur dur");
			SDLNet_TCP_Close(client2);
			break;

		}
		while (client) {


			

			SDLNet_TCP_Send(client, client2_posx, sizeof(client2_posx));
			//SDLNet_TCP_Send(client2, client_posx, sizeof(client_posx));
			SDLNet_TCP_Send(client, client2_posy, sizeof(client2_posy));
			//SDLNet_TCP_Send(client2, client_posy, sizeof(client_posy));
			SDL_Delay(1);


			printf("kek");

			SDLNet_TCP_Close(client);
			break;
		}
	}
	SDLNet_TCP_Close(server);

	SDLNet_Quit();
	SDL_Quit();


}
