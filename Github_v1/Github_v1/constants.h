#pragma once
#ifndef constants_h
#define constants_h

#define BALL_WIDTH 20
#define BALL_HEIGHT 20
#define SCREEN_W 1000
#define SCREEN_H 500
#define PAD_WIDTH 20
#define PAD_HEIGHT 100
#define BALL_SPEED 5
#define PLAYER_SPEED 1
#define LOCAL_PORT 1234

//const float PAD_INIT_Y0 = 0;
const float NET_TICK_RATE = 60;
const float ballSpeed = BALL_SPEED; //OOF
const float p1_posX0 = 0, p1_posY0 = 0;
const float p2_posX0 = SCREEN_W, p2_posY0 = 0;
const float BALL_INIT_X0 = SCREEN_W / 2;
const float BALL_INIT_Y0 = SCREEN_H / 2;
typedef struct ballpacket {
	float yPos;
	float xPos;
	int h_direction;
	int v_direction;
	int yangle;
}ball_packet;

typedef struct IPpacket
{
	float ball_yPos;
	float ball_xPos;
	int h_dir;
	int v_dir;
	int yangle;
	float pad_Y, pad_X; // byt till pad_y, pad_x??
	int which_player;
} IP_packet_t;

typedef struct players
{
	float yPos; // använd snake för mer consistency 
	float xPos;
}Players;
#endif // !1
