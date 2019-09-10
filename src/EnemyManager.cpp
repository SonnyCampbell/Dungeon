#include "EnemyManager.h"

using namespace AnimatedSpriteManager;

namespace EnemyManager
{
Enemy *NewEnemy1(SDL_Renderer **renderer, Vec2 position, float speed)
{
    Vec2 frameSize = {16, 16}; //TODO Constants - Spritesheet frame width/height ---- ^ also --16:28 is default
    int runFps = 10;
    int idleFps = 5;

    auto *animations = new std::map<AnimationKey, Animation *>(
        {{IdleUp, new Animation(4, idleFps, frameSize, {369, 35})},
         {WalkUp, new Animation(4, runFps, frameSize, {369, 35})}});

    auto rb = RigidBody(60.f, 16.f, 20.f, position, speed, Vec2(0, 0));

    auto sprite = NewAnimatedSprite(renderer, "assets/DungeonTilesetV2.png", animations, IdleUp);
    EntityStats stats = {100, speed};
    Enemy *enemy = new Enemy{Game::NextEntityId(), stats, sprite, rb, nullptr};

    return enemy;
}

void DeleteEnemy(Enemy &enemy)
{
    if (enemy.weapon)
    {
        WeaponManager::DeleteWeapon(enemy.weapon);
    }

    if (enemy.sprite)
    {
        AnimatedSpriteManager::DeleteAnimatedSprite(*enemy.sprite);
    }

    delete &enemy;
}

void TakeDamage(Enemy &enemy, int damage)
{
    enemy.stats.health -= damage;
    enemy.rb.aabb.center += Vec2(5.f, 0.f);
    printf("OW! Health: %i \n", enemy.stats.health);
}

void UpdateEnemy(Enemy &enemy, Player &player)
{
    SimpleAI::Update(enemy, player);
    enemy.rb.aabb.center = enemy.rb.aabb.center + (enemy.rb.direction * enemy.rb.speed * Game::tick_delta());

    UpdateAnimation(*enemy.sprite, enemy.sprite->currentAnimationKey);
}

void DrawEnemy(Enemy &enemy)
{
    auto camera_position = Vec2(Game::camera.x, Game::camera.y);
    DrawSprite(*enemy.sprite, enemy.rb.aabb.min() - camera_position);
}
} // namespace EnemyManager
