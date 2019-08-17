#pragma once
#include "Vec2.h"
class AABB
{
private:
public:
    Vec2 halfExtents;
    Vec2 center;

    AABB(Vec2 _halfExtents, Vec2 _center)
    {
        halfExtents = _halfExtents;
        center = _center;
    }

    ~AABB()
    {
    }

    Vec2 min()
    {
        return Vec2(center.x() - halfExtents.x(), center.y() - halfExtents.y());
    }

    Vec2 max()
    {
        return Vec2(center.x() + halfExtents.x(), center.y() + halfExtents.y());
    }

    Vec2 size()
    {
        return Vec2(halfExtents.x() * 2.f, halfExtents.y() * 2.f);
    }
};

class RigidBody
{
private:
public:
    float mass;
    float inverseMass;
    AABB aabb;
    float speed;
    Vec2 direction;

    RigidBody(float _mass, float width, float height, Vec2 center, float _speed, Vec2 _direction) : aabb(Vec2(width / 2.f, height / 2.f), center)
    {
        mass = _mass;
        inverseMass = mass != 0 ? 1.f / mass : 0;
        direction = _direction;
        speed = _speed;
    }
    ~RigidBody()
    {
    }
};