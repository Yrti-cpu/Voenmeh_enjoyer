#pragma once
#include "Game.h"
class SoundManager
{
public:
	static Mix_Music* LoadMusic(const char* path);
	static Mix_Chunk* LoadSound(const char* path);
};

