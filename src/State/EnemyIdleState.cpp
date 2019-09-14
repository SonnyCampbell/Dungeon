#include "EnemyIdleState.h"

EnemyIdleState::EnemyIdleState()
{
}
EnemyIdleState::~EnemyIdleState()
{
}

EnemyState *EnemyIdleState::update(Enemy &enemy)
{
    printf("Idling... \n");
}

void EnemyIdleState::enterState(Enemy &enemy)
{
}

void EnemyIdleState::exitState(Enemy &enemy)
{
}
