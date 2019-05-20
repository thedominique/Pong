#pragma once
#ifndef soundeffect_h
#define soundeffect_h
#include <SDL_mixer.h>
#include "objects.h"
//Ljudeffekter hittade http://soundbible.com/1120-Bounce.html //

typedef struct sound {
	Mix_Music *soundeffect;
	int channel;
}sound;

void initsound(sound *soundeffect);

#endif	soundeffect_h