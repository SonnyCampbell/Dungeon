#include "EnemyIdleState.h"

EnemyIdleState::EnemyIdleState()
{
}
EnemyIdleState::~EnemyIdleState()
{
}

void EnemyIdleState::update(Enemy &enemy)
{
    printf("Idling... \n");
}

void EnemyIdleState::enterState(Enemy &enemy)
{
}

void EnemyIdleState::exitState(Enemy &enemy)
{
}
