#pragma once

#include "..\AnimatedSprite.h"
#include "..\RigidBody.h"
#include "..\Weapon.h"
#include "..\EntityStats.h"

struct Enemy
{
    int id;
    EntityStats stats;
    AnimatedSprite *sprite;
    RigidBody rb;
    Weapon *weapon;
};