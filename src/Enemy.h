#pragma once
#include "AnimatedSprite.h"
#include "Vec2.h"
#include "RigidBody.h"
#include "Weapon.h"
#include "Game Components/Game.h"
#include "EntityStats.h"

struct Enemy
{
    EntityStats stats;
    AnimatedSprite *sprite;
    RigidBody rb;
    Weapon *weapon;
};

namespace EnemyManager
{
Enemy NewEnemy1(SDL_Renderer **renderer, Vec2 position, float speed = 166.0f);
void UpdateEnemy(Enemy &enemy, double current_tick, float dt);
void DrawEnemy(Enemy &enemy);
}
