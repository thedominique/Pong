#pragma once
#ifndef player_handler_h
#define player_handler_h
#include "objects.h"


void identify_player(UDPpacket *packet_receive, IPaddress client_ipaddress[], GameState *gamestate)
{
	int i = 0;

	for (i = 0; i < 4; i++)
	{
		if (packet_receive->address.host == client_ipaddress[i].host
			&& packet_receive->address.port == client_ipaddress[i].port)
		{
			Paddle* receive = (Paddle*)packet_receive->data;
			gamestate->players[i].x = receive->x;
			gamestate->players[i].y = receive->y;
			//printf("Player %d made a move", i);
			//printf("RE: %lf \n", receive->x);

		}
	}
}
#endif // !player_handler_h