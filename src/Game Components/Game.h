#pragma once

#include <vector>
#include "SDL.h"
#include "../Vec2.h"

struct DebugRect
{
    SDL_FRect rect;
    SDL_Colour colour;
};

struct Game
{
    static std::vector<DebugRect> debug_rects;

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
