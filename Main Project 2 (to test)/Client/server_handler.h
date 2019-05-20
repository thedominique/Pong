#ifndef server_handler_h
#define server_handler_h
#include "objects.h"

void receive_server_values(UDPpacket *packet_receive, GameState *gamestate);
#endif // !server_handler_h