#pragma once
#include "AnimatedSprite.h"
#include "Vec2.h"
#include "RigidBody.h"
#include "Weapon.h"
#include "Game Components/Game.h"
#include "EntityStats.h"
#include "Enemies/Enemy.h"
#include "Enemies/Attacks/E1BasicAttack.h"
#include "SimpleAI.h"
#include "Player.h"
#include "State/FSMTableState.h"

namespace EnemyManager
{
Enemy NewEnemy1(SDL_Renderer **renderer, Vec2 position, float speed = 15.0f);
void DeleteEnemyById(std::vector<Enemy> &enemies, int id);
} // namespace EnemyManager
