#include "SimpleAI.h"

namespace SimpleAI
{
void Update(Enemy &enemy, Player &player)
{
    //If enemy can see player
    //  move towards player
    auto separation_vector = player.rb.aabb.center - enemy.rb.aabb.center;
    auto dist = separation_vector.length();
    if (dist < 50 && dist > 8)
    {
        enemy.rb.direction = separation_vector.normalized_vector();
    }
    else
    {
        enemy.rb.direction = Vec2::zero();
    }

    //else
    // move back towards starting position
}
} // namespace SimpleAI