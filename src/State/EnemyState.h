#pragma once

class Enemy;

class EnemyState
{
public:
    virtual ~EnemyState() {}
    virtual void update(Enemy &enemy) {}
    virtual void enterState(Enemy &enemy) {}
    virtual void exitState(Enemy &enemy) {}
};