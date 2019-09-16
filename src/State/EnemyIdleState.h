#pragma once
#include "EnemyState.h"
#include "../Structs/Enemy.h"

class EnemyIdleState : public EnemyState
{
public:
    EnemyIdleState();
    virtual ~EnemyIdleState();
    virtual void update(Enemy &enemy);
    virtual void enterState(Enemy &enemy);
    virtual void exitState(Enemy &enemy);
};