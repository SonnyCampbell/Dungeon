#pragma once
#include <vector>
#include <algorithm>

#include "TMXLoader/TMXTileSet.h"
#include "TMXLoader/TMXTileLayer.h"

struct BroadphaseTile
{
    unsigned int tileId;
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

Vec2 tileToWorld(int x, int y, TMXTileSet &tileset)
{
    return Vec2((float)x * tileset.getTileWidth(), (float)y * tileset.getTileHeight());
}

AABB tileToAABB(int x, int y, TMXTileSet &tileset)
{
    Vec2 tile = tileToWorld(x, y, tileset);
    Vec2 extents = Vec2(tileset.getTileWidth() / 2.f, tileset.getTileHeight() / 2.f);

    return AABB(tile + extents, extents);
}

bool isInternalCollision(unsigned int tileX, unsigned int tileY, Vec2 normal, TMXTileLayer &tileLayer)
{
    auto next_tile_X = tileX + (int)normal.x();
    auto next_tile_y = tileY + (int)normal.y();

    //auto current_tile = tileLayer.getTileVector()[tileX][tileY];
    //auto next_tile = tileLayer.getTileVector()[nextTileX][nextTileY];
    return tileLayer.getTileId(next_tile_X, next_tile_y) > 0;

    // let isInternalCollision (tileX: int) (tileY: int) (normal: Vector2) (tileLayer: TiledMapTileLayer) =
    // let nextTileX = tileX + int normal.X
    // let nextTileY = tileY + int normal.Y

    // let currentTile = TiledMapTileLayer.getTileId tileX tileY tileLayer
    // let nextTile = TiledMapTileLayer.getTileId nextTileX nextTileY tileLayer

    // match nextTile with None -> false | Some _ -> true
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
    if (!internal_collision)
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

            auto tileAABB = tileToAABB(x, y, tileset);
            auto tileId = tileLayer.getTileVector()[x][y];

            tiles.push_back({tileId, tileAABB, x, y});
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
    // let solved = Speculative.speculativeSolver dt contact

    //     solved.a.velocity, contact
    return CollisionResponse({true, solved.a.velocity(), contact});
}

CollisionResponse innerCollide(TMXTileLayer &tileLayer, RigidBody moveableObject, AABB &tileAABB, int tileType, float dt, int x, int y, SDL_Renderer *renderer)
{
    auto tileRb = RigidBody(0.f, tileAABB.size().x(), tileAABB.size().y(), tileAABB.center, 0.f, Vec2::zero());
    auto collision_contact = AABBvAABB(moveableObject, tileRb, x, y, tileLayer);

    auto tileWidth = tileAABB.size().x();
    auto tileHeight = tileAABB.size().y();

    SDL_FRect srcrect = {((tileType - 1) % 32) * tileWidth, ((tileType - 1) / 32) * tileHeight, tileWidth, tileHeight}; // TODO Constant 32 = tiles wide/high (2 extra = layer around map?)
    auto destX = x * tileWidth;
    auto destY = y * tileHeight;
    SDL_FRect debugRect = {destX, destY, tileWidth, tileHeight};

    if (collision_contact.first)
    {
        SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
        SDL_RenderDrawRectF(renderer, &debugRect);

        return collisionResponse(dt, collision_contact.second);
    }

    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
    SDL_RenderDrawRectF(renderer, &debugRect);

    return CollisionResponse();
    // let tileRigidBody = RigidBody.create(0.f, tileAabb.size.X, tileAabb.size.Y, tileAabb.center, Vector2.Zero)
    //         let collision, contact = AABBvAABB moveableObject tileRigidBody x y tileLayer

    //         if collision then
    //             //let pixelCollision = intersectsPixels
    //             Some (collisionResponse moveableObject tileRigidBody contact dt)
    //         else None
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
        auto tile = tiles[i];
        if (tile.tileId == 0)
            continue;

        auto collision = innerCollide(tileLayer, rb, tile.tileAABB, tile.tileId, dt, tile.x, tile.y, renderer);
        if (collision.collision)
        {
            collisionResponses.push_back(collision);
        }
    }

    return collisionResponses;

    //     |> List.choose(fun (tileid, tileaabb, x,y) -> innerCollide map rigidBody tileaabb tileid dt x y)
}

// let broadphaseTiles =
//     [for x in minX..maxX do
//          for y in minY..maxY do
//              let tileaabb = TiledMapTileset.toAABB x y tileset
//              let tileId = TiledMapTileLayer.getTileId x y tileLayer

//              yield tileId, tileaabb, x, y]

// broadphaseTiles

} // namespace Collision
