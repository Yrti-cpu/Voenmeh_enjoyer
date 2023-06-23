#pragma once
#include <vector>
#include "Loot.h"
#include "Map.h"
#include "Player.h"
#include "Text.h"
#include <typeinfo>

class Manager
{
private:
	std::vector<GameObject*> gameObjects;
public:
	void update()//обновление всех сущностей и их компонентов
	{

		for (auto& o : gameObjects)
		{
			if (Game::isRunning == 7)
			{
				Text t;
				if (typeid(*o).name() == typeid(t).name())
				{
					continue;
				}
			}
			o->update();
		}
	}
	bool isEmpty() { return gameObjects.empty(); }
	bool isText(int i)
	{
		Text t;
		return typeid(*gameObjects[i]).name() == typeid(t).name();
	}
	GameObject* get_GameObject(int i) { return gameObjects[i]; }
	size_t get_size() { return gameObjects.size(); }
	void draw()//отрисовка всех сущностей
	{
		for (auto& o : gameObjects) o->draw();
	}

	void refresh()//проверка активности сущности (к примеру, проверка жив или мертв игрок или крип)
	{
		gameObjects.erase(std::remove_if(std::begin(gameObjects), std::end(gameObjects),
			[](GameObject* mGameObject)
			{
				return !mGameObject->isActive();
			}),
			std::end(gameObjects));
	}

	void addGameObject(int xpos, int ypos, int time, char n)//добавление сущности
	{
		GameObject* o = new Loot(xpos, ypos, time, n);
		gameObjects.emplace_back(std::move(o));
	}
	void addMap(char n)
	{
		GameObject* m = new Map(n);
		gameObjects.emplace_back(std::move(m));
	}
	void addPlayer(Player* p)
	{
		gameObjects.emplace_back(std::move(p));
	}
	void addText(int xpos, int ypos, const wchar_t* message, SDL_Color color)
	{
		GameObject* t = new Text(xpos, ypos, message, color);
		gameObjects.emplace_back(std::move(t));
	}
};

