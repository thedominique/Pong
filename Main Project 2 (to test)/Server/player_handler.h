#ifndef player_handler_h
#define player_handler_h
#include "objects.h"


void  get_client_values(UDPpacket *packet_receive, IPaddress client_ipaddress[], GameState *gamestate);
void reset_game(GameState *gamestate);
int check_players_started(GameState *gamestate);
#endif // !collision_h