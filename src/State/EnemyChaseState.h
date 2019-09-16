#pragma once
#include "EnemyState.h"
#include "../Structs/Enemy.h"

class EnemyChaseState : public EnemyState
{
public:
    EnemyChaseState();
    virtual ~EnemyChaseState();
    virtual void update(Enemy &enemy);
    virtual void enterState(Enemy &enemy);
    virtual void exitState(Enemy &enemy);
};