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
#include "State/FSMTableState.h"

namespace EnemyManager
{
Enemy NewEnemy1(SDL_Renderer **renderer, FSMTableState::StateMachineData &rb_states, std::vector<AnimatedSprite *> &sprites, Vec2 position, float speed = 6.0f);
void DeleteEnemy(Enemy &enemy);
void UpdateEnemy(Enemy &enemy, Player &player);
void DrawEnemy(Enemy &enemy, RigidBody rb);
void TakeDamage(Enemy &enemy, int damage);
} // namespace EnemyManager
