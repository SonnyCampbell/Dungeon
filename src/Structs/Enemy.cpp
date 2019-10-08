#include "Enemy.h"

void Enemy::draw()
{
    auto camera_position = Vec2(Game::camera.x, Game::camera.y);
    AnimatedSpriteManager::DrawSprite(*sprite, rb.aabb.min() - camera_position);
};

void Enemy::update()
{
    //enemy.rb.aabb.center = enemy.rb.aabb.center + (enemy.rb.direction * enemy.rb.speed * Game::tick_delta());

    AnimatedSpriteManager::UpdateAnimation(*sprite, sprite->currentAnimationKey);
}

void Enemy::takeDamage(int damage)
{
    stats.health -= damage;
    printf("OW! Health: %i \n", stats.health);
}

void Enemy::deleteEnemy()
{
    if (weapon)
    {
        WeaponManager::DeleteWeapon(weapon);
    }

    if (sprite)
    {
        AnimatedSpriteManager::DeleteAnimatedSprite(*sprite);
    }
}