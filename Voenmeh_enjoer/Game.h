#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include <list>
#include "Records.h"

class Game
{
public:
	Game();//констурктор
	~Game();//деструктор

	void init(const char* title, int width, int height, bool fullscreen);//инициализация окна и рендеринга
	void menu();
	void start();
	void end();
	void info();
	void player_name();
	void records(std::list<Records*>& r);
	void handleEvents();//обработка действий пользователя
	void generateGameObject();
	void update();//обновление состояния игры
	void render();//рендеринг картинки 
	void clean();//закрытие окна и рендеринга 

	int running() { return isRunning; }

	static SDL_Renderer* renderer;//структура, в которой хранится состояние рендеринга
	static TTF_Font* gFont;
	static int isRunning;
	static int score;
	static std::wstring name;
	static int SelectedItem;


	static bool Flag1;
	static bool Flag2;

	enum Status
	{
		OFF,
		PAUSE,
		MENU,
		END,
		INFO,
		PLAYER_NAME,
		RECORDS,
		ON
	};
private:
	SDL_Window* window;//окно, в котором хранится вся отображаемая графика
	SDL_Event event;//обработка событий

	Mix_Music* gMusic;
	Mix_Music* gMenu;
	Mix_Music* gWin;
	Mix_Music* gLose;

	Mix_Chunk* gSelect;
	Mix_Chunk* gStart;
	Mix_Chunk* gItem;
	Mix_Chunk* gScroll;

};