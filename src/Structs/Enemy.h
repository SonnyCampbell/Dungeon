#pragma once

#include "..\AnimatedSprite.h"
#include "..\RigidBody.h"
#include "..\Weapon.h"
#include "..\EntityStats.h"
#include "..\Game Components\Game.h"

enum EnemyState1
{
    idle,
    chasing,
    attacking
};

class Enemy
{
public:
    int id;
    EntityStats stats;
    Weapon *weapon;
    AnimatedSprite *sprite;
    RigidBody rb;
    EnemyState1 state;

    void draw();
    void update();
    void takeDamage(int damage);
    void deleteEnemy();
};