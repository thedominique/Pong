#pragma once
#ifndef game_state_h
#define game_state_h
typedef struct gamestate {
	float ball_yPos;
	float ball_xPos;
	float ball_Xspeed;
	float ball_Yspeed;
	float p_yPos, p_xPos;
	int which_player;
} gamestate_t;

typedef struct player {
	float yPos, xPos;
} Players_t;

Players_t player[3];

long ticks_per_sec;
long tick_t0;

long next_net_tick;
long net_tick_interval;


void init_Tick();


#endif // !1