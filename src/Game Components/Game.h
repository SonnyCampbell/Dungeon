#include "SDL.h"
#include "../Vec2.h"

namespace Game
{
extern SDL_Rect camera;

void UpdateCamera(Vec2 player_position, int screen_width, int screen_height, int level_width, int level_height);
} // namespace Game