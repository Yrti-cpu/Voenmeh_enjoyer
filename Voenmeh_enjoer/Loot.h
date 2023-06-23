#pragma once
#include "GameObject.h"
#include <chrono>

class Loot :
    public GameObject
{
private:
    std::chrono::steady_clock::time_point isActive_time;
    int time;

public:
    Loot(int xpos, int ypos, int time, char n)
    {
        isActive_time = std::chrono::steady_clock::now();
        this->time = time;
        position.x = xpos;
        position.y = ypos;
        active = true;
        if (n == 0)
        {
            setTex("sprites/Chapa.png");
            srcRect.h = 17;
            srcRect.w = 14;
            point = -10;
        }
        else if (n == 1)
        {
            setTex("sprites/money.png");
            srcRect.h = 8;
            srcRect.w = 16;
            point = 7;
        }
        else if (n == 2)
        {
            setTex("sprites/Coffee.png");
            srcRect.h = 19;
            srcRect.w = 16;
            point = 5;
        }
        else if (n == 3)
        {
            setTex("sprites/Chiken_burger.png");
            srcRect.h = 14;
            srcRect.w = 24;
            point = 2;
        }
        else if (n == 4)
        {
            setTex("sprites/Vitamix.png");
            srcRect.h = 32;
            srcRect.w = 12;
            point = 6;
        }
        else if (n == 5)
        {
            setTex("sprites/3.png");
            srcRect.h = 7;
            srcRect.w = 6;
            point = -2;
            scale = 6;
        }
        else if (n == 6)
        {
            setTex("sprites/4.png");
            srcRect.h = 7;
            srcRect.w = 5;
            point = 8;
            scale = 6;
        }
        else if (n == 7)
        {
            setTex("sprites/5.png");
            srcRect.h = 7;
            srcRect.w = 5;
            point = 12;
            scale = 6;
        }

        srcRect.x = 0;
        srcRect.y = 0;
        destRect.x = static_cast<int>(position.x);
        destRect.y = static_cast<int>(position.y);

        destRect.w = srcRect.w * scale;
        destRect.h = srcRect.h * scale;
    }

    void update() override
    {
        if ((std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - isActive_time).count() >= time))
        {
            std::cout << "out of range" << std::endl;
            destroy();
        }
    }



    ~Loot()
    {
        SDL_DestroyTexture(texture);
    }
};

