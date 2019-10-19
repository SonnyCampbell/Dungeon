#pragma once
#include "EnemyManager.h"
#include "Enemies/Enemy.h"
#include "Player.h"
#include "Vec2.h"
#include "State/EnemyIdleState.h"
#include "State/EnemyChaseState.h"
namespace SimpleAI
{
// void MakeDecision(Enemy &enemy, Player &player);
void Update(Enemy &enemy, Player &player);
}