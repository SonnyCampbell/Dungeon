#include "EnemyManager.h"

using namespace AnimatedSpriteManager;

namespace EnemyManager
{
Enemy NewEnemy1(SDL_Renderer **renderer, Vec2 position, float speed)
{
    Vec2 frameSize = {16, 15}; //TODO Constants - Spritesheet frame width/height ---- ^ also --16:28 is default
    int runFps = 10;
    int idleFps = 5;

    auto *animations = new std::map<AnimationKey, Animation *>(
        {{IdleUp, new Animation(4, idleFps, frameSize, {370, 37})},
         {WalkUp, new Animation(4, runFps, frameSize, {370, 37})}});
    int id = Game::NextEntityId();
    auto rb = new RigidBody(id, 60.f, 12.f, 12.f, position, speed, Vec2(0, 0));
    //rb_states.idles.push_back(rb);

    auto sprite = NewAnimatedSprite(renderer, id, "assets/DungeonTilesetV2.png", animations, IdleUp);
    //sprites.push_back(*sprite);
    EntityStats stats = {100, speed};
    Enemy *enemy = new Enemy{id, stats, nullptr, sprite, *rb};

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
    enemy.deleteEnemy();
    delete &enemy;
}

} // namespace EnemyManager
