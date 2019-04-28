#include "player_handler.h"

void identify_player(UDPpacket *packet_receive, IPaddress client_ipaddress[], GameState *gamestate)
{
	int i = 0;

	for (i = 0; i < 3; i++)
	{
		if (packet_receive->address.host == client_ipaddress[i].host
			&& packet_receive->address.port == client_ipaddress[i].port)
		{
			Paddle* receive = (Paddle*)packet_receive->data;

			gamestate->keys[i].d = receive->k.d;
			gamestate->keys[i].a = receive->k.a;
			gamestate->keys[i].w = receive->k.w;
			gamestate->keys[i].s = receive->k.s;
			gamestate->keys[i].up = receive->k.up;
			gamestate->keys[i].down = receive->k.down;
			//printf("Player %d made a move", i);

		}
	}
}