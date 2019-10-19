#pragma once
#include "Vec2.h"
#include "SDL.h"
class AABB
{
private:
public:
    Vec2 halfExtents;
    Vec2 center;

    AABB()
    {
    }

    AABB(Vec2 _center, Vec2 _halfExtents)
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

    SDL_Rect boundingBox()
    {
        return {(int)min().x(), (int)min().y(), (int)halfExtents.x() * 2, (int)halfExtents.y() * 2};
    }

    SDL_FRect boundingBoxF()
    {
        return {min().x(), min().y(), halfExtents.x() * 2, halfExtents.y() * 2};
    }
};

class RigidBody
{
private:
public:
    int entity_id;
    float mass;
    float inverseMass;
    AABB aabb;
    float speed;
    Vec2 direction;

    RigidBody()
    {
    }

    RigidBody(int id, float _mass, float width, float height, Vec2 center, float _speed, Vec2 _direction) : aabb(center, Vec2(width / 2.f, height / 2.f))
    {
        entity_id = id;
        mass = _mass;
        inverseMass = mass != 0 ? 1.f / mass : 0;
        direction = _direction;
        speed = _speed;
    }
    ~RigidBody()
    {
    }

    Vec2 velocity()
    {
        return direction * speed;
    }
};
