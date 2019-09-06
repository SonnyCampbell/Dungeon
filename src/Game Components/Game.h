#pragma once

#include "SDL.h"
#include "../Vec2.h"

struct Game
{
    static SDL_Rect camera;
    static int screen_width;
    static int screen_height;

    static int current_tick;
    static int last_tick;
    static float tick_delta();

    static float render_scale;

    static int level_width;
    static int level_height;

    static void UpdateCamera(Vec2 player_position);
    static int NextEntityId();
};
