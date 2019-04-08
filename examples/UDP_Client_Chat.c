#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SDL_net.h"

int main(int argc, char **argv)
{

	SDLNet_Init();
	UDPsocket client_socket;
	IPaddress ipaddress;
	UDPpacket *packet;
	int quit;


	/* Open a socket on random port */
	client_socket = SDLNet_UDP_Open(1234);

	/* Resolve server name  */
	SDLNet_ResolveHost(&ipaddress, "130.229.152.251", 1234);


	/* Allocate memory for the packet */
	(packet = SDLNet_AllocPacket(512));


	/* Main loop */
	quit = 0;
	while (!quit)
	{
		if (SDLNet_UDP_Recv(client_socket, packet))
		{
			printf("Mail: %s", (char *)packet->data);
		}
		printf("Fill the buffer\n>");
		scanf("%s", (char *)packet->data);

		//packet->address = ipaddress;
		packet->address.host = ipaddress.host;	/* Set the destination host */
		packet->address.port = ipaddress.port;	/* And destination port */

		packet->len = strlen((char *)packet->data) + 1;
		SDLNet_UDP_Send(client_socket, -1, packet); /* This sets the packet->channel */



		/* Quit if packet contains "quit" */
		if (!strcmp((char *)packet->data, "quit"))
		{
			quit = 1;
		}



	}

	SDLNet_FreePacket(packet);
	SDLNet_Quit();

	return EXIT_SUCCESS;
}
