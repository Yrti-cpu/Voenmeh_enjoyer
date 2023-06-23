#pragma once
#include "GameObject.h"
#include "Animation.h"
#include <map>
class Player :
	public GameObject
{
private:
	int frames = 0;
	int speedAnim = 100;

public:
	Vector2D velocity;
	int speed = 4;
	int animIndex = 0;
	std::map<const char*, Animation> animations;

	Player()
	{
		active = true;
		Animation Idle = Animation(0, 4, 100);
		Animation WalkDown = Animation(1, 6, 100);
		Animation WalkRight = Animation(2, 6, 100);
		Animation WalkUp = Animation(3, 6, 100);
		Animation WalkLeft = Animation(4, 6, 100);

		animations.emplace("Idle", Idle);
		animations.emplace("WalkDown", WalkDown);
		animations.emplace("WalkRight", WalkRight);
		animations.emplace("WalkUp", WalkUp);
		animations.emplace("WalkLeft", WalkLeft);
		position.x = 350;
		position.y = 300;
		active = true;
		setTex("sprites/anim.png");
		Play("Idle");
		srcRect.h = 32;
		srcRect.w = 48;
		srcRect.x = static_cast<int>(position.x);
		srcRect.y = static_cast<int>(position.y);
	}

	void update() override
	{
		position.x += velocity.x * speed;
		position.y += velocity.y * speed;


		srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speedAnim) % frames);
		srcRect.y = animIndex * srcRect.h;

		destRect.x = static_cast<int>(position.x);
		destRect.y = static_cast<int>(position.y);
		destRect.w = srcRect.w * scale;
		destRect.h = srcRect.h * scale;

	}
	void Play(const char* animName)
	{
		frames = animations[animName].frames;
		animIndex = animations[animName].index;
		speedAnim = animations[animName].speed;
	}


	~Player()
	{
		SDL_DestroyTexture(texture);
	}
};

