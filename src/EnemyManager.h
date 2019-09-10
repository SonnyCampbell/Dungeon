#pragma once
#include "AnimatedSprite.h"
#include "Vec2.h"
#include "RigidBody.h"
#include "Weapon.h"
#include "Game Components/Game.h"
#include "EntityStats.h"
#include "Structs/Enemy.h"
#include "SimpleAI.h"
#include "Player.h"

namespace EnemyManager
{
Enemy *NewEnemy1(SDL_Renderer **renderer, Vec2 position, float speed = 6.0f);
void DeleteEnemy(Enemy &enemy);
void UpdateEnemy(Enemy &enemy, Player &player);
void DrawEnemy(Enemy &enemy);
void TakeDamage(Enemy &enemy, int damage);
} // namespace EnemyManager
