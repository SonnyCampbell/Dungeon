#include "EnemyManager.h"

using namespace AnimatedSpriteManager;

namespace EnemyManager
{
Enemy NewEnemy1(SDL_Renderer **renderer, FSMTableState::StateMachineData &rb_states, std::vector<AnimatedSprite> &sprites,
                Vec2 position, float speed)
{
    Vec2 frameSize = {16, 16}; //TODO Constants - Spritesheet frame width/height ---- ^ also --16:28 is default
    int runFps = 10;
    int idleFps = 5;

    auto *animations = new std::map<AnimationKey, Animation *>(
        {{IdleUp, new Animation(4, idleFps, frameSize, {369, 35})},
         {WalkUp, new Animation(4, runFps, frameSize, {369, 35})}});
    int id = Game::NextEntityId();
    auto rb = RigidBody(id, 60.f, 16.f, 20.f, position, speed, Vec2(0, 0));
    rb_states.idles.push_back(rb);

    auto sprite = NewAnimatedSprite(renderer, id, "assets/DungeonTilesetV2.png", animations, IdleUp);
    sprites.push_back(*sprite);
    EntityStats stats = {100, speed};
    Enemy *enemy = new Enemy{id, stats, nullptr};

    return *enemy;
}

void DeleteEnemyById(std::vector<Enemy> &enemies, int id)
{
    auto it = std::find_if(enemies.begin(), enemies.end(), [id](const Enemy val) {
        return val.id == id;
    });

    if (it == enemies.end())
        return;

    Enemy &enemy = *it;
    if (enemy.weapon)
    {
        WeaponManager::DeleteWeapon(enemy.weapon);
    }

    /* if (enemy.sprite)
    {
        AnimatedSpriteManager::DeleteAnimatedSprite(*enemy.sprite);
    } */

    delete &enemy;
}

void DeleteEnemy(Enemy &enemy)
{
    if (enemy.weapon)
    {
        WeaponManager::DeleteWeapon(enemy.weapon);
    }

    /* if (enemy.sprite)
    {
        AnimatedSpriteManager::DeleteAnimatedSprite(*enemy.sprite);
    } */

    delete &enemy;
}

void TakeDamage(Enemy &enemy, int damage)
{
    enemy.stats.health -= damage;
    printf("OW! Health: %i \n", enemy.stats.health);
}

void UpdateEnemy(AnimatedSprite &sprite, Player &player)
{
    //enemy.rb.aabb.center = enemy.rb.aabb.center + (enemy.rb.direction * enemy.rb.speed * Game::tick_delta());

    UpdateAnimation(sprite, sprite.currentAnimationKey);
}

void DrawEnemy(AnimatedSprite &sprite, RigidBody rb)
{
    auto camera_position = Vec2(Game::camera.x, Game::camera.y);
    DrawSprite(sprite, rb.aabb.min() - camera_position);
}
} // namespace EnemyManager
