#include "soundeffect.h"

void initsound(sound *soundeffect) {
	Mix_Init(MIX_INIT_MP3);
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 8, 4096) == -1) {
		printf("Audio failed to open %s", SDL_GetError());
	}

	soundeffect->soundeffect = Mix_LoadWAV("Bounce.wav");

	if (soundeffect->soundeffect == NULL) {
		printf("soundeffect failed to load %s", SDL_GetError());
	}

	return;
}

void play_soundeffect(sound *soundeffect, GameState *gamestate)

{
	if (gamestate->ball.collision == 1)
	{
		printf("ball collision: %d", gamestate->ball.collision);
		Mix_PlayChannel(2, soundeffect->soundeffect, 0);
		gamestate->ball.collision = 0;
		printf("ball collision: %d", gamestate->ball.collision);
	}

}