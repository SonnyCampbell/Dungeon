#include "Game.h"

namespace Game
{
void UpdateCamera(Vec2 player_position, int screen_width, int screen_height, int level_width, int level_height)
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
    if (camera.x > (level_width - camera.w) / 1.5)
    {
        camera.x = (level_width - camera.w) / 1.5;
    }
    if (camera.y > (level_height - camera.h) / 1.5)
    {
        camera.y = (level_height - camera.h) / 1.5;
    }
}
} // namespace Game