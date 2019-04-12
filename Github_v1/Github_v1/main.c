#include <stdio.h>
#include <SDL.h>
#include <SDL_net.h>
#include <string.h>
#include <stdbool.h>
#include "game_state.h"
#include "constants.h"
#include "client.h"
#include "server.h"

bool address_equal(IPaddress a, IPaddress b);
void print_ip(int ip);	// bara för att identifiera att rätt maskin hittad

int main(int argc, char** argv[]) {
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
			clientState();
		}
		else if (hesitation == 2)
		{
			printf("Initiate as server...\n");
			SDL_Delay(1000);
			// INSERT SERVER FUNCTION
			serverState();
		}
		
	} while (hesitation != 0);

	SDLNet_Quit();
	SDL_Quit();

	return 0;
}
