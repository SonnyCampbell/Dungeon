#include "Enemy.h"

void Enemy::draw()
{
    auto camera_position = Vec2(Game::camera.x, Game::camera.y);
    AnimatedSpriteManager::DrawSprite(*sprite, rb.aabb.min() - camera_position);
    //DEBUG DRAWING
    SDL_FRect debug_rect = {this->rb.aabb.min().x() - Game::camera.x, this->rb.aabb.min().y() - Game::camera.y, (float)this->rb.aabb.size().x(), (float)this->rb.aabb.size().y()};
    SDL_Colour colour = {0x00, 0x00, 0xFF, 0xFF};
    Game::debug_rects.push_back({debug_rect, colour});
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

void Enemy::takeHit(int distance, Vec2 &srcPosition)
{
    auto separation_vector = srcPosition - this->rb.aabb.center;
    auto position_update = separation_vector.normalized_vector() * distance;
    this->rb.aabb.center -= position_update;
    return;
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

void Enemy::beginAttack(Player &player)
{
    this->attack.readying_attack = true;
    this->attack.target_position = player.rb.aabb.center;
}

void Enemy::continueAttack(Player &player)
{
    if (Game::current_tick - this->attack.last_frame_time > this->attack.frame_length)
    {
        this->rb.direction = this->attack.attack_prep[this->attack.ready_frame];
        this->rb.aabb.center = this->rb.aabb.center + (this->rb.direction * this->rb.speed * Game::tick_delta());
        this->attack.last_frame_time = Game::current_tick;

        this->attack.ready_frame++;
        if (this->attack.ready_frame >= 4)
        {
            this->attack.ready_frame = 0;
            this->attack.readying_attack = false;
        }
    }
}

void Enemy::finishAttack(Player &player)
{
    auto separation_vector = this->attack.target_position - this->rb.aabb.center;
    this->rb.aabb.center += separation_vector.normalized_vector() * 30; //make this a property of the attack/enemy
    printf("Finished attack \n");
}