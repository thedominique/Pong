#pragma once
#ifndef server_handler_h
#define server_handler_h
#include "objects.h"


void receive_server_values(UDPpacket *packet_receive, GameState *gamestate)
{
	Receive *receive = (Receive *)packet_receive->data;

	gamestate->ball.x = receive->b1.x;
	gamestate->ball.y = receive->b1.y;

	gamestate->players[0].x = receive->p[0].x;
	gamestate->players[0].y = receive->p[0].y;

	gamestate->players[1].x = receive->p[1].x;
	gamestate->players[1].y = receive->p[1].y;
}




#endif // !server_handler_h

