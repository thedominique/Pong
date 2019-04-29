#include "server_handler.h"
#include "soundeffect.h"

void receive_server_values(UDPpacket *packet_receive, GameState *gamestate, sound *soundeffect) {
	Receive *receive = (Receive *)packet_receive->data;

	gamestate->ball.x = receive->b1.x;
	gamestate->ball.y = receive->b1.y;
	gamestate->ball.collision = receive->b1.collision;
	if (gamestate->ball.collision == 1) {
		printf("%d", gamestate->ball.collision);
		soundeffect->channel = Mix_PlayChannel(3, soundeffect->soundeffect, 0);
		gamestate->ball.collision = 0;
	}

	gamestate->players[0].x = receive->p[0].x;
	gamestate->players[0].y = receive->p[0].y;
	gamestate->players[0].lives = receive->p[0].lives;
	gamestate->players[0].w = receive->p[0].w;
	gamestate->players[0].h = receive->p[0].h;

	gamestate->players[1].x = receive->p[1].x;
	gamestate->players[1].y = receive->p[1].y;
	gamestate->players[1].lives = receive->p[1].lives;

	gamestate->players[1].w = receive->p[1].w;
	gamestate->players[1].h = receive->p[1].h;

	gamestate->players[2].x = receive->p[2].x;
	gamestate->players[2].y = receive->p[2].y;
	gamestate->players[2].lives = receive->p[2].lives;
	gamestate->players[2].w = receive->p[2].w;
	gamestate->players[2].h = receive->p[2].h;


	gamestate->ball.redShade = receive->b1.redShade;
	gamestate->ball.greenShade = receive->b1.greenShade;
	gamestate->ball.blueShade = receive->b1.blueShade;
}
