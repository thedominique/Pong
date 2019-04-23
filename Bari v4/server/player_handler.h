#pragma once
#ifndef player_handler_h
#define player_handler_h
#include "objects.h"


void identify_player(UDPpacket *packet_receive, IPaddress client_ipaddress[], GameState *gamestate);
