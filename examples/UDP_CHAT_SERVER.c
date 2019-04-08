#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL_net.h"

int main(int argc, char **argv)
{

	SDLNet_Init();
	UDPsocket server_socket;
	IPaddress server_ipaddress;
	UDPpacket *packet;
	int quit=0;


	/* Open a socket on random port */
	server_socket = SDLNet_UDP_Open(1234);

	/* Resolve server name  */
	SDLNet_ResolveHost(&server_ipaddress, NULL, 1234);


	/* Allocate memory for the packet */
	packet = SDLNet_AllocPacket(512);

	while (1)
	{

		if (SDLNet_UDP_Recv(server_socket, packet))
		{
			printf("Du har fått något %s\n",(char *) packet->data);


		}
		printf("Write test");
		scanf("%s", (char*)packet->data);
		SDLNet_UDP_Send(server_socket,-1, packet);
	}



	SDLNet_FreePacket(packet);
	SDLNet_Quit();
}


