#pragma once

#include "SDL.h"
#include "../Vec2.h"

class Game
{
private:
public:
    static SDL_Rect camera;
    static int screen_width;
    static int screen_height;

    static float render_scale;

    static int level_width;
    static int level_height;

    static void UpdateCamera(Vec2 player_position);
};
