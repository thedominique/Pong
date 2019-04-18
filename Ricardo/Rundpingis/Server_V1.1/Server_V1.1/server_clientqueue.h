#pragma once
#ifndef server_clientqueue_h
#define server_clientqueue_h
#include "SDL_net.h"
#include <stdbool.h>
#include <SDL.h>


void init_array_struct(IPaddress client_ipaddress[])
{
	int i = 0;

	for (i = 0; i < 4; i++)
	{
		client_ipaddress[i].host = 0;
		client_ipaddress[i].port = 0;

	}
}

bool address_equal(IPaddress a, IPaddress b) {
	return a.host == b.host && a.port == b.port;
}

void save_ip(UDPpacket *packet, IPaddress client_ipaddress[])
{
	int i = 0;

	for (i = 0; i < 4; i++)
	{
		if (client_ipaddress[i].host == 0 && client_ipaddress[i].port == 0)
		{
			client_ipaddress[i].host = packet->address.host;
			client_ipaddress[i].port = packet->address.port;
			//printf("client 0s host: %d port : %d\n ", client_ipaddress[0].host, client_ipaddress[0].port);
			//printf("client 1s host: %d port : %d\n ", client_ipaddress[1].host, client_ipaddress[1].port);
			//printf("client 2s host: %d port : %d\n ", client_ipaddress[2].host, client_ipaddress[2].port);
			//printf("end of shit\n");
			return;
		}
	}
}

int check_ipaddress(UDPpacket *packet, IPaddress client_ipaddress[])
{
	int i = 0;
	int ip_exist = 0;
	for (i = 0; i < 4; i++)
	{
		if (address_equal(packet->address, client_ipaddress[i]))
		{
			//printf("already exist\n");
			ip_exist = 1;
		}

	}

	if (!ip_exist)
	{
		save_ip(packet, client_ipaddress);
	}

	return ip_exist;
}
#endif // !server_clientqueue_h