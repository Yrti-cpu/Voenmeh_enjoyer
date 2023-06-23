#pragma once
#include "GameObject.h"
#include "Collision.h"
#include <string>

class Text :
    public GameObject
{
private:
    const wchar_t* message;
    SDL_Rect* rect = &srcRect;
    int id;
    bool change;

public:
    static int cnt;
    Text(int xpos, int ypos, const wchar_t* message, SDL_Color color)
    {
        position.x = xpos;
        position.y = ypos;
        active = true;
        change = true;
        this->message = message;
        setT(message, color, rect);

        srcRect.x = 0;
        srcRect.y = 0;

        destRect.x = static_cast<int>(position.x);
        destRect.y = static_cast<int>(position.y);

        destRect.w = srcRect.w;
        destRect.h = srcRect.h;
        id = cnt;
        cnt++;
    }
    Text()
    {

    }
    void update() override
    {
        if (Game::isRunning != 7)
        {
            if (Game::SelectedItem == id)
            {
                if (change)
                {
                    SDL_Color color = { 255, 0, 0 };
                    setT(message, color, rect);
                    change = false;
                }

            }
            else
            {
                if (!change)
                {
                    SDL_Color color = { 0, 0, 0 };
                    setT(message, color, rect);
                    change = true;
                }

            }
        }

    }

    ~Text()
    {
        SDL_DestroyTexture(texture);
    }
};

