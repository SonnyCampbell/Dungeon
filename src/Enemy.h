#pragma once
#include "AnimatedSprite.h"
#include "Vec2.h"
#include "RigidBody.h"
#include "Weapon.h"
#include "Game Components/Game.h"
#include "EntityStats.h"

struct Enemy
{
    int id;
    EntityStats stats;
    AnimatedSprite *sprite;
    RigidBody rb;
    Weapon *weapon;
};

namespace EnemyManager
{
Enemy *NewEnemy1(SDL_Renderer **renderer, Vec2 position, float speed = 166.0f);
void UpdateEnemy(Enemy &enemy);
void DrawEnemy(Enemy &enemy);
void TakeDamage(Enemy &enemy, int damage);
} // namespace EnemyManager
