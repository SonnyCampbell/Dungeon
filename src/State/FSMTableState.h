#pragma once
#include <vector>
#include "../Vec2.h"
#include "../Player.h"
#include "../Structs/Enemy.h"
#include "../EnemyManager.h"

namespace FSMTableState
{
typedef std::vector<RigidBody> EntityRbVector;

struct StateMachineData
{
    EntityRbVector idles;
    EntityRbVector chasings;

    StateMachineData()
    {
        idles = EntityRbVector();
        chasings = EntityRbVector();
    }

    EntityRbVector AllEnemies()
    {
        EntityRbVector all_enemies = EntityRbVector();

        all_enemies.insert(all_enemies.end(), idles.begin(), idles.end());
        all_enemies.insert(all_enemies.end(), chasings.begin(), chasings.end());
        return all_enemies;
    }

    void DeleteEnemyState(int enemy_id)
    {
        auto it = std::find_if(idles.begin(), idles.end(),
                               [enemy_id](const RigidBody val) {
                                   return val.entity_id == enemy_id;
                               });

        if (it != idles.end())
        {
            idles.erase(it);
            return;
        }

        it = std::find_if(chasings.begin(), chasings.end(),
                          [enemy_id](const RigidBody val) {
                              return val.entity_id == enemy_id;
                          });

        if (it != chasings.end())
        {
            chasings.erase(it);
            return;
        }
    }

    void UpdateEnemyPosition(int enemy_id, int distance, Player &player)
    {
        auto it = std::find_if(idles.begin(), idles.end(),
                               [enemy_id](const RigidBody val) {
                                   return val.entity_id == enemy_id;
                               });

        if (it != idles.end())
        {
            auto separation_vector = player.rb.aabb.center - it->aabb.center;
            auto position_update = separation_vector.normalized_vector() * distance;
            it->aabb.center -= position_update;
            return;
        }

        it = std::find_if(chasings.begin(), chasings.end(),
                          [enemy_id](const RigidBody val) {
                              return val.entity_id == enemy_id;
                          });

        if (it != chasings.end())
        {
            auto separation_vector = player.rb.aabb.center - it->aabb.center;
            auto position_update = separation_vector.normalized_vector() * distance;
            it->aabb.center -= position_update;
            return;
        }
    }

    void UpdateStates(float deltaTime, Player &player)
    {
        EntityRbVector pendingChase = EntityRbVector();
        EntityRbVector pendingIdle = EntityRbVector();

        for (auto iter = idles.begin(); iter != idles.end();)
        {
            printf("%i idling...\n", iter->entity_id);
            auto separation_vector = player.rb.aabb.center - iter->aabb.center;
            auto dist = separation_vector.length();

            if (dist < 100 && dist > 8)
            {
                printf("%i taking chase..\n", iter->entity_id);
                iter->direction = separation_vector.normalized_vector();

                iter->aabb.center = iter->aabb.center + (iter->direction * iter->speed * Game::tick_delta());
                pendingChase.push_back(*iter);
                *iter = idles.back();
                idles.pop_back();
            }
            else
            {
                ++iter;
            }
        }

        for (auto iter = chasings.begin(); iter != chasings.end();)
        {
            printf("%i chasing...\n", iter->entity_id);
            auto separation_vector = player.rb.aabb.center - iter->aabb.center;
            auto dist = separation_vector.length();

            if (dist >= 100 || dist <= 8)
            {
                printf("%i lost sight..\n", iter->entity_id);
                iter->direction = Vec2::zero();
                pendingIdle.push_back(*iter);
                *iter = chasings.back();
                chasings.pop_back();
            }
            else
            {
                iter->direction = separation_vector.normalized_vector();
                iter->aabb.center = iter->aabb.center + (iter->direction * iter->speed * Game::tick_delta());
                ++iter;
            }
        }

        idles.insert(idles.end(), pendingIdle.begin(), pendingIdle.end());
        chasings.insert(chasings.end(), pendingChase.begin(), pendingChase.end());
    }
};

} // namespace FSMTableState