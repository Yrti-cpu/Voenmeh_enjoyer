#include "SoundManager.h"

Mix_Music* SoundManager::LoadMusic(const char* path)
{
	Mix_Music* music = Mix_LoadMUS(path);
	if (music == NULL)
	{
		printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
		Game::isRunning = Game::OFF;
	}
	if (Game::isRunning)
	{
		std::cout << "music " << path << " loaded!\n";
	}
	return music;
}

Mix_Chunk* SoundManager::LoadSound(const char* path)
{
	Mix_Chunk* sound = Mix_LoadWAV(path);
	if (sound == NULL)
	{
		printf("Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		Game::isRunning = Game::OFF;
	}
	if (Game::isRunning)
	{
		std::cout << "sound " << path << " loaded!\n";
	}
	return sound;
}
