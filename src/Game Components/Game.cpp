#include "Game.h"

int Game::screen_width = 600;
int Game::screen_height = 600;
float Game::render_scale = 1.5;
int Game::level_width = 480 * render_scale;
int Game::level_height = 480 * render_scale;
SDL_Rect Game::camera = SDL_Rect{100, 100, screen_width, screen_height};

void Game::UpdateCamera(Vec2 player_position)
{
    //Center the camera over the dot
    camera.x = (player_position.x()) - screen_width / 2;
    camera.y = (player_position.y()) - screen_height / 2;

    //Keep the camera in bounds
    if (camera.x < 0)
    {
        camera.x = 0;
    }
    if (camera.y < 0)
    {
        camera.y = 0;
    }
    if (camera.x > (level_width - camera.w) / render_scale)
    {
        camera.x = (level_width - camera.w) / render_scale;
    }
    if (camera.y > (level_height - camera.h) / render_scale)
    {
        camera.y = (level_height - camera.h) / render_scale;
    }
}