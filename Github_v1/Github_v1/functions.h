#pragma once
#ifndef functions_h
#define functions_h
#include <stdbool.h>
#include <SDL_net.h>
#include "constants.h"

int sort_clients(IPaddress packet, IPaddress client_addr[], int* new_client) {
	int which_player = 0;
	if (packet.host == client_addr[0].host)
	{
		printf("Hej Client 1!\n");
		client_addr[0].host = packet.host;
		client_addr[0].port = packet.port;
		which_player = 1;
	}
	else if (packet.host == client_addr[1].host)
	{
		//printf("Hej Client 2!\n");
		client_addr[1].host = packet.host;
		client_addr[1].port = packet.port;
		which_player = 2;
	}
	else if (packet.host == client_addr[2].host)
	{
		//printf("Hej Client 3!\n");
		which_player = 3;
	}
	else if ((*new_client) == 3)
	{
		printf("Full\n");
	}
	else {
		printf("Valkommen client %d!\n", (*new_client));
		client_addr[(*new_client)].host = packet.host;
		client_addr[(*new_client)].port = packet.port;
		(*new_client) = (*new_client) + 1;
	}
	return which_player;
}

void wallhit(ball_packet ball[]) {
	int random = 0;
	if (ball[0].xPos >= SCREEN_W)
	{
		ball[0].h_direction = 2;
		if (ball[0].v_direction == 1 || ball[0].v_direction == 0)
		{
			ball[0].v_direction = 1;
		}
		else ball[0].v_direction = 2;
	}

	if (ball[0].xPos <= 0)
	{
		ball[0].h_direction = 1;
		if (ball[0].v_direction == 2 || ball[0].v_direction == 0)
		{
			ball[0].v_direction = 2;
		}
		else
			ball[0].v_direction = 1;
	}

	if (ball[0].yPos <= 0)
	{
		//horizontal_direction = 1;
		ball[0].v_direction = 2;
		ball[0].yangle = rand() % 4 + 1;

	}
	if (ball[0].xPos >= SCREEN_H)
	{
		//horizontal_direction = 1;
		ball[0].v_direction = 1;
		ball[0].yangle = rand() % 4 + 1;
	}
}

void moveball(ball_packet ball[], double dt) {
	//printf("angle = %d\n", ball[0].yangle);
	//printf("H = %d\n", ball[0].h_direction);
	//printf("V = %d\n", ball[0].v_direction);
	int yangle = 0;
	int v_dir = 0;
	int h_dir = 0;

	yangle = ball[0].yangle;
	v_dir = ball[0].v_direction;
	h_dir = ball[0].h_direction;

	if (ball[0].h_direction == 2)
	{
		ball[0].xPos -= (200 * dt);
		//printf("change\n");
	}

	if (ball[0].h_direction == 1)
	{
		ball[0].xPos += 200 * dt;
		//printf("change\n");
	}

	if (ball[0].v_direction == 2)
	{
		//printf("change\n");
		ball[0].yPos += ball[0].yangle * dt;
	}

	if (ball[0].v_direction == 1)
	{
		//printf("change\n");
		ball[0].yPos -= ball[0].yangle * dt;
	}
}

void padhit(Players player[], ball_packet ball[]) {
	// BLOCK BALL RIGHT PADDLE

	for (int i = 0; i < 2; i++)
	{
		if (ball[0].xPos < player[i].xPos - 10 && ball[0].xPos > player[i].xPos - 16 && ball[0].yPos >= player[i].yPos - 45 && ball[0].yPos <= player[i].yPos + 145)
		{
			ball[0].h_direction = 2;
			if (ball[0].v_direction == 1 || ball[0].v_direction == 0)
			{
				ball[0].v_direction = 1;
			}
			else
				ball[0].v_direction = 2;
			break;
		}

		// BLOCK BALL LEFT PADDLE

		if (player[i].xPos + 10 < ball[0].xPos && player[i].xPos + 16 > ball[0].xPos && player[i].yPos - 45 < ball[0].yPos && player[i].yPos + 145 > ball[0].yPos)
		{
			ball[0].h_direction = 1;
			if (ball[0].v_direction == 1 || ball[0].v_direction == 0)
			{
				ball[0].v_direction = 1;
			}
			else
				ball[0].v_direction = 2;
		}
	}
}
bool address_equal(IPaddress a, IPaddress b) {
	return a.host == b.host && a.port == b.port;
}
#endif // !1