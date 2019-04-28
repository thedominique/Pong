#ifndef server_clientqueue_h
#define server_clientqueue_h


#include "objects.h"

bool address_equal(IPaddress a, IPaddress b);
void init_array_struct(IPaddress client_ipaddress[]);
void save_ip(UDPpacket *packet, IPaddress client_ipaddress[], GameState *gamestate);
int check_ipaddress(UDPpacket *packet, IPaddress client_ipaddress[], GameState *gamestate);

#endif // !server_clientqueue_h