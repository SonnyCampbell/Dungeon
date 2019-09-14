#include "EnemyChaseState.h"

EnemyChaseState::EnemyChaseState()
{
}
EnemyChaseState::~EnemyChaseState()
{
}

EnemyState *EnemyChaseState::update(Enemy &enemy)
{
    printf("Chasing... \n");
}

void EnemyChaseState::enterState(Enemy &enemy)
{
}

void EnemyChaseState::exitState(Enemy &enemy)
{
}
