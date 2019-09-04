#pragma once
#include <vector>
#include <algorithm>

#include "Game Components/Game.h"
#include "TMXLoader/TMXTileSet.h"
#include "TMXLoader/TMXTileLayer.h"

struct BroadphaseTile
{
    TMXTile tile;
    AABB tileAABB;
    int x;
    int y;
};

class Contact
{
private:
public:
    RigidBody a;
    RigidBody b;
    Vec2 normal;
    float distance;
    float impulse;

    Contact()
    {
    }

    Contact(RigidBody _a, RigidBody _b, Vec2 _normal, float _distance, float _impulse) : a(_a), b(_b)
    {
        normal = _normal;
        distance = _distance;
        impulse = _impulse;
    }

    ~Contact()
    {
    }
};

struct CollisionResponse
{
    bool collision;
    Vec2 velocity;
    Contact contact;
};

namespace Collision
{

Vec2 tileToWorld(int x, int y, int w, int h, TMXTile &tile)
{
    return Vec2((float)(x * w) + tile.getCollisionBoundary().x, (float)(y * h) + tile.getCollisionBoundary().y);
}

AABB tileToAABB(int x, int y, int w, int h, TMXTile &tile)
{
    Vec2 tile_in_world = tileToWorld(x, y, w, h, tile);
    Vec2 extents = Vec2(tile.getCollisionBoundary().w / 2.f, tile.getCollisionBoundary().h / 2.f);

    return AABB(tile_in_world + extents, extents);
}

bool isInternalCollision(unsigned int tileX, unsigned int tileY, Vec2 normal, TMXTileLayer &tileLayer)
{
    auto next_tile_X = tileX + (int)normal.x();
    auto next_tile_y = tileY + (int)normal.y();

    //auto current_tile = tileLayer.getTileVector()[tileX][tileY];
    //auto next_tile = tileLayer.getTileVector()[nextTileX][nextTileY];
    return tileLayer.getTileId(next_tile_X, next_tile_y) > 0;
}

bool checkBoxCollision(SDL_Rect a, SDL_Rect b)
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
    if (bottomA <= topB)
    {
        return false;
    }

    if (topA >= bottomB)
    {
        return false;
    }

    if (rightA <= leftB)
    {
        return false;
    }

    if (leftA >= rightB)
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}

std::pair<bool, Contact> AABBvAABB(RigidBody a, RigidBody b, int collision_distance, unsigned int tileX, unsigned int tileY, TMXTileLayer &layer)
{
    auto combined_extents = a.aabb.halfExtents + b.aabb.halfExtents;
    auto delta = b.aabb.center - a.aabb.center;

    auto normal = -delta.major_axis();
    auto plane_center = (normal * combined_extents) + b.aabb.center;

    auto plane_delta = a.aabb.center - plane_center;
    auto dist = Vec2::dot_product(plane_delta, normal);

    auto internal_collision = isInternalCollision(tileX, tileY, normal, layer);
    if (!internal_collision && dist < collision_distance)
    {
        auto contact = Contact(a, b, normal, dist, 0.f);
        return std::pair<bool, Contact>(true, contact);
    }

    return std::pair<bool, Contact>(false, Contact());
}

std::pair<bool, Contact> AABBvAABB(RigidBody a, RigidBody b, unsigned int tileX, unsigned int tileY, TMXTileLayer &layer)
{
    auto combined_extents = a.aabb.halfExtents + b.aabb.halfExtents;
    auto delta = b.aabb.center - a.aabb.center;

    auto normal = -delta.major_axis();
    auto plane_center = (normal * combined_extents) + b.aabb.center;

    auto plane_delta = a.aabb.center - plane_center;
    auto dist = Vec2::dot_product(plane_delta, normal);

    auto internal_collision = isInternalCollision(tileX, tileY, normal, layer);
    if (!internal_collision && dist < 5)
    {
        auto contact = Contact(a, b, normal, dist, 0.f);
        return std::pair<bool, Contact>(true, contact);
    }

    return std::pair<bool, Contact>(false, Contact());
}

std::vector<BroadphaseTile> getBroadphaseTiles(TMXTileLayer &tileLayer, TMXTileSet &tileset, const Vec2 &min, const Vec2 &max)
{
    int minX = (int)min.x() / tileset.getTileWidth();
    int minY = (int)min.y() / tileset.getTileHeight();

    int maxX = (int)((max.x() + 0.5f) / tileset.getTileWidth());
    int maxY = (int)((max.y() + 0.5f) / tileset.getTileHeight());

    std::vector<BroadphaseTile> tiles = std::vector<BroadphaseTile>();

    for (int x = minX; x <= maxX; x++)
    {
        if (x < 0 || x >= tileLayer.getWidth())
            continue;
        for (int y = minY; y <= maxY; y++)
        {
            if (y < 0 || y >= tileLayer.getHeight())
                continue;

            auto tile = tileLayer.getTileVector()[x][y];
            auto tileAABB = tileToAABB(x, y, tileset.getTileWidth(), tileset.getTileHeight(), *tile);

            tiles.push_back({*tile, tileAABB, x, y});
        }
    }

    return tiles;
}

Contact speculativeSolver(float dt, Contact contact)
{
    auto normal = -contact.normal;
    auto nv = Vec2::dot_product(contact.b.velocity() - contact.a.velocity(), normal);

    if (nv > 0.f)
    {
        return contact;
    }

    auto remove = nv + (contact.distance / dt);
    auto impulse = remove / (contact.a.inverseMass + contact.b.inverseMass);

    auto new_impulse = std::min(impulse + contact.impulse, 0.f);

    auto change = new_impulse - contact.impulse;

    auto new_contact = Contact(contact);

    auto a_velocity = (change * normal * contact.a.inverseMass);
    auto b_velocity = (change * normal * contact.b.inverseMass);

    new_contact.a.speed += a_velocity.length();
    new_contact.a.direction += a_velocity.normalized_vector();
    new_contact.b.speed -= b_velocity.length();
    new_contact.b.direction -= b_velocity.normalized_vector();
    new_contact.impulse = new_impulse;

    return new_contact;
}

CollisionResponse collisionResponse(float dt, Contact contact)
{
    auto solved = speculativeSolver(dt, contact);

    return CollisionResponse({true, solved.a.velocity(), contact});
}

CollisionResponse innerCollide(TMXTileLayer &tileLayer, TMXTileSet &tileset, RigidBody moveableObject, AABB &tileAABB, TMXTile tile, float dt, int x, int y, SDL_Renderer *renderer)
{
    auto tileRb = RigidBody(0.f, tile.getCollisionBoundary().w, tile.getCollisionBoundary().h, tileAABB.center, 0.f, Vec2::zero());
    auto collision_contact = AABBvAABB(moveableObject, tileRb, tile.getCollisionDistance(), x, y, tileLayer);

    if (collision_contact.first)
    {
        return collisionResponse(dt, collision_contact.second);
    }

    return CollisionResponse();
}

std::vector<CollisionResponse> collision(TMXTileLayer &tileLayer, TMXTileSet &tileset, RigidBody rb, float dt, SDL_Renderer *renderer)
{
    auto expand = Vec2(0.f, 0.f);
    auto predictedPos = rb.aabb.center + (rb.velocity() * dt);

    // find min/max
    auto min = Vec2::min(rb.aabb.center, predictedPos);
    auto max = Vec2::max(rb.aabb.center, predictedPos);

    // extend by radius and extend more to deal with being close to boundary
    min = min - rb.aabb.halfExtents - expand;
    max = max + rb.aabb.halfExtents + expand;

    auto tiles = getBroadphaseTiles(tileLayer, tileset, min, max);

    auto collisionResponses = std::vector<CollisionResponse>();
    for (int i = 0; i < tiles.size(); i++)
    {
        auto bp_tile = tiles[i];
        if (bp_tile.tile.getTileID() == 0)
            continue;

        auto collision = innerCollide(tileLayer, tileset, rb, bp_tile.tileAABB, bp_tile.tile, dt, bp_tile.x, bp_tile.y, renderer);

        auto tileWidth = bp_tile.tileAABB.size().x();
        auto tileHeight = bp_tile.tileAABB.size().y();
        auto tileType = bp_tile.tile.getTileID();

        SDL_FRect srcrect = {((tileType - 1) % 32) * tileWidth, ((tileType - 1) / 32) * tileHeight, tileWidth, tileHeight}; // TODO Constant 32 = tiles wide/high (2 extra = layer around map?)
        auto destX = bp_tile.x * tileset.getTileWidth();
        auto destY = bp_tile.y * tileset.getTileHeight();
        SDL_FRect debugRect = {(float)destX - Game::camera.x, (float)destY - Game::camera.y, (float)tileset.getTileWidth(), (float)tileset.getTileHeight()};

        if (collision.collision)
        {
            SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
            SDL_RenderDrawRectF(renderer, &debugRect);
            auto bounds = bp_tile.tile.getCollisionBoundary();
            SDL_FRect collision_rect = {(float)destX + bounds.x - Game::camera.x, (float)destY + bounds.y - Game::camera.y, (float)bounds.w, (float)bounds.h};
            SDL_RenderDrawRectF(renderer, &collision_rect);

            collisionResponses.push_back(collision);
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
            SDL_RenderDrawRectF(renderer, &debugRect);
        }
    }

    return collisionResponses;
}

} // namespace Collision
