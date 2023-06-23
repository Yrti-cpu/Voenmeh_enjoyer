#pragma once
#include "GameObject.h"
class Map :
    public GameObject
{
public:
    Map(char n)
    {
        active = true;
        position.x = 0;
        position.y = 0;
        if (n == 0)
            setTex("sprites/preview.png");
        else
            setTex("sprites/map.png");
        srcRect.h = 270;
        srcRect.w = 480;
        scale = 4;
        srcRect.x = 0;
        srcRect.y = 0;
        destRect.x = static_cast<int>(position.x);
        destRect.y = static_cast<int>(position.y);

        destRect.w = srcRect.w * scale;
        destRect.h = srcRect.h * scale;
    }

    void update(void) override {}

};

