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
