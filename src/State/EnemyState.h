#pragma once

class Enemy;

class EnemyState
{
public:
    virtual ~EnemyState() {}
    virtual EnemyState *update(Enemy &enemy) {}
    virtual void enterState(Enemy &enemy) {}
    virtual void exitState(Enemy &enemy) {}
};