#pragma once
#include "Game.h"
#include "TextureManager.h"
#include "Vector2D.h"
class GameObject
{
public:

	virtual ~GameObject() {};
	virtual void update() = 0;
	void destroy() { active = false; }
	bool isActive() { return active; }
	SDL_Rect get_Rect() { return destRect; }
	int get_point() { return point; }
	Vector2D get_pos() { return position; }
	int get_ypos() { return position.y; }
	int get_xpos() { return position.x; }
	void set_xpos(int xpos) { position.x = xpos; }
	void set_ypos(int ypos) { position.y = ypos; }
	void setTex(const char* path)
	{
		texture = TextureManager::LoadTexture(path);
	}
	void draw()
	{
		TextureManager::Draw(texture, srcRect, destRect);
	}
	void setT(const wchar_t* message, SDL_Color color, SDL_Rect* rect)
	{
		texture = TextureManager::LoadText(message, color, rect);
	}

protected:
	Vector2D position;
	int point, scale = 3;
	bool active;
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;
};

