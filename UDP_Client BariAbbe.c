#if 0
#!/bin/sh

gcc - Wall `sdl-config --cflags` udpc.c - o udpc `sdl-config --libs` - lSDL_net

exit
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SDL_net.h"

int main(int argc, char **argv)
{

	SDLNet_Init();
	UDPsocket clientSocket;
	IPaddress ipaddress;
	UDPpacket *packet;

	int quit;


	/* Open a socket on random port */
	clientSocket = SDLNet_UDP_Open(1666);

	/* Resolve server name  */
	SDLNet_ResolveHost(&ipaddress, "192.168.0.5", 1234);


	/* Allocate memory for the packet */
	(packet = SDLNet_AllocPacket(512));


	/* Main loop */
	quit = 0;
	while (!quit)
	{
		printf("Fill the buffer\n>");
		scanf("%s", (char *)packet->data);

		packet->address.host = ipaddress.host;	/* Set the destination host */
		packet->address.port = ipaddress.port;	/* And destination port */

		packet->len = strlen((char *)packet->data) + 1;
		SDLNet_UDP_Send(clientSocket, -1, packet); /* This sets the packet->channel */



		/* Quit if packet contains "quit" */
		if (!strcmp((char *)packet->data, "quit"))
			quit = 1;

		if (SDLNet_UDP_Recv(clientSocket, packet))
		{
			printf("%s\n", (char*)packet->data);
		}

	}

	SDLNet_FreePacket(packet);
	SDLNet_Quit();

	return EXIT_SUCCESS;
}
