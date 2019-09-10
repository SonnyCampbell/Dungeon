#pragma once
#include "EnemyManager.h"
#include "Structs/Enemy.h"
#include "Player.h"
#include "Vec2.h"

namespace SimpleAI
{
// void MakeDecision(Enemy &enemy, Player &player);
void Update(Enemy &enemy, Player &player);
}