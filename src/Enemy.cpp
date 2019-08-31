#include "Enemy.h"

using namespace AnimatedSpriteManager;

namespace EnemyManager
{
Enemy NewEnemy1(SDL_Renderer **renderer, Vec2 position, float speed)
{
    Vec2 frameSize = {12, 13}; //TODO Constants - Spritesheet frame width/height ---- ^ also --16:28 is default
    int runFps = 10;
    int idleFps = 5;

    auto *animations = new std::map<AnimationKey, Animation *>({{IdleUp, new Animation(4, idleFps, frameSize, {370, 19})},
                                                                {WalkUp, new Animation(4, runFps, frameSize, {370, 19})}});

    auto rb = RigidBody(60.f, 16.f, 20.f, position, speed, Vec2(0, 0));

    auto sprite = NewAnimatedSprite(renderer, "assets/DungeonTilesetV2.png", animations, WalkUp);

    Enemy enemy = {sprite, rb, nullptr};

    return enemy;
}

void DrawEnemy(Enemy enemy)
{
    auto camera_position = Vec2(Game::camera.x, Game::camera.y);
    DrawSprite(*enemy.sprite, enemy.rb.aabb.min() - camera_position);
}
} // namespace EnemyManager