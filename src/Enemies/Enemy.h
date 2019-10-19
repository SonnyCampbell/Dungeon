#pragma once

#include "..\AnimatedSprite.h"
#include "..\RigidBody.h"
#include "..\Weapon.h"
#include "..\EntityStats.h"
#include "..\Game Components\Game.h"
#include ".\Attacks\E1BasicAttack.h"
#include "..\Player.h"

enum EnemyState1
{
    idle,
    chasing,
    readying_attack,
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
    E1BasicAttack attack;
    EnemyState1 state;

    void draw();
    void update();
    void takeDamage(int damage);
    void takeHit(int distance, Vec2 &srcPosition);
    void deleteEnemy();
    void beginAttack(Player &player);
    void continueAttack(Player &player);
    void finishAttack(Player &player);
};