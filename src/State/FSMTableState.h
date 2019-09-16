#pragma once
#include <vector>
#include "../Vec2.h"
#include "../Player.h"
#include "../Structs/Enemy.h"
#include "../EnemyManager.h"

namespace FSMTableState
{
typedef std::vector<Enemy> EnemyVector;

struct StateMachineData
{
    EnemyVector idles;
    EnemyVector chasings;

    StateMachineData()
    {
        idles = EnemyVector();
        chasings = EnemyVector();
    }

    EnemyVector AllEnemies()
    {
        EnemyVector all_enemies = EnemyVector();

        all_enemies.insert(all_enemies.end(), idles.begin(), idles.end());
        all_enemies.insert(all_enemies.end(), chasings.begin(), chasings.end());
        return all_enemies;
    }

    void DeleteEnemyState(int enemy_id)
    {
        // For now this deletes the whole enemy, but when it's properly
        // DoD it will only delete the relevant information
        auto it = std::find_if(idles.begin(), idles.end(),
                               [enemy_id](const Enemy val) {
                                   return val.id == enemy_id;
                               });

        if (it != idles.end())
        {
            idles.erase(it);
            EnemyManager::DeleteEnemy(*it);
            return;
        }

        it = std::find_if(chasings.begin(), chasings.end(),
                          [enemy_id](const Enemy val) {
                              return val.id == enemy_id;
                          });

        if (it != chasings.end())
        {
            chasings.erase(it);
            EnemyManager::DeleteEnemy(*it);
            return;
        }
    }

    void UpdateStates(float deltaTime, Player &player)
    {
        EnemyVector pendingChase = EnemyVector();
        EnemyVector pendingIdle = EnemyVector();

        for (auto iter = idles.begin(); iter != idles.end();)
        {
            printf("%i idling...\n", iter->id);
            auto separation_vector = player.rb.aabb.center - iter->rb.aabb.center;
            auto dist = separation_vector.length();

            if (dist < 50 && dist > 8)
            {
                printf("%i taking chase..\n", iter->id);
                iter->rb.direction = separation_vector.normalized_vector();

                iter->rb.aabb.center = iter->rb.aabb.center + (iter->rb.direction * iter->rb.speed * Game::tick_delta());
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
            printf("%i chasing...\n", iter->id);
            auto separation_vector = player.rb.aabb.center - iter->rb.aabb.center;
            auto dist = separation_vector.length();

            if (dist >= 50 || dist <= 8)
            {
                printf("%i lost sight..\n", iter->id);
                iter->rb.direction = Vec2::zero();
                pendingIdle.push_back(*iter);
                *iter = chasings.back();
                chasings.pop_back();
            }
            else
            {
                iter->rb.direction = separation_vector.normalized_vector();
                iter->rb.aabb.center = iter->rb.aabb.center + (iter->rb.direction * iter->rb.speed * Game::tick_delta());
                ++iter;
            }
        }

        idles.insert(idles.end(), pendingIdle.begin(), pendingIdle.end());
        chasings.insert(chasings.end(), pendingChase.begin(), pendingChase.end());
    }
};

} // namespace FSMTableState