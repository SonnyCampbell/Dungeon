#pragma once
#include <vector>
#include "../Vec2.h"
#include "../Player.h"
#include "../Structs/Enemy.h"
#include "../EnemyManager.h"

namespace FSMTableState
{
struct Attack
{
    int entity_id;
    Vec2 target_position;
    int last_frame_time;
    Vec2 attack_prep[4] = {Vec2(30.f, 0.f), Vec2(-30.f, 0.f), Vec2(30.f, 0.f), Vec2(-30.f, 0.f)};
    bool readying_attack = true;
    int ready_frame = 0;
    float frame_length = 60.f;
    bool is_attacking = false;
    float attack_speed = 60.f;
};

typedef std::vector<RigidBody> EntityRbVector;
typedef std::vector<Attack> AttackVectors;

struct StateMachineData
{
    EntityRbVector idles;
    EntityRbVector chasings;
    EntityRbVector attacking;
    AttackVectors attacks;

    StateMachineData()
    {
        idles = EntityRbVector();
        chasings = EntityRbVector();
        attacking = EntityRbVector();
        attacks = AttackVectors();
    }

    EntityRbVector AllEnemies()
    {
        EntityRbVector all_enemies = EntityRbVector();

        all_enemies.insert(all_enemies.end(), idles.begin(), idles.end());
        all_enemies.insert(all_enemies.end(), chasings.begin(), chasings.end());
        all_enemies.insert(all_enemies.end(), attacking.begin(), attacking.end());
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

        //TODO: Check attacking vector
    }

    void UpdateStates(float deltaTime, Player &player)
    {
        EntityRbVector pendingChase = EntityRbVector();
        EntityRbVector pendingIdle = EntityRbVector();
        EntityRbVector pendingAttack = EntityRbVector();

        for (auto iter = idles.begin(); iter != idles.end();)
        {
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
            auto separation_vector = player.rb.aabb.center - iter->aabb.center;
            auto dist = separation_vector.length();

            if (dist >= 100)
            {
                printf("%i lost sight..\n", iter->entity_id);
                iter->direction = Vec2::zero();
                pendingIdle.push_back(*iter);
                *iter = chasings.back();
                chasings.pop_back();
            }
            else if (dist < 20)
            {
                printf("%i attacking player \n", iter->entity_id);
                iter->direction = Vec2::zero();
                pendingAttack.push_back(*iter);
                attacks.push_back({iter->entity_id, player.rb.aabb.center, Game::current_tick});
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

        int i = 0;
        for (auto iter = attacking.begin(); iter != attacking.end();)
        {
            Attack &attack = attacks[i];
            if (attack.readying_attack)
            {
                if (Game::current_tick - attack.last_frame_time > attack.frame_length)
                {
                    printf("Readying attack \n");
                    iter->direction = attack.attack_prep[attack.ready_frame];
                    iter->aabb.center = iter->aabb.center + (iter->direction * iter->speed * Game::tick_delta());
                    attack.last_frame_time = Game::current_tick;

                    attack.ready_frame++;
                    if (attack.ready_frame >= 4)
                    {
                        attack.readying_attack = false;
                        attack.is_attacking = true;
                    }
                }

                ++iter;
            }
            else
            {
                auto separation_vector = player.rb.aabb.center - iter->aabb.center;
                iter->aabb.center += separation_vector.normalized_vector() * 1000 * Game::tick_delta();
                printf("Finished attack \n");
                pendingIdle.push_back(*iter);
                *iter = attacking.back();
                attacking.pop_back();
                std::swap(attacks[i], attacks.back());
                attacks.pop_back();
            }

            ++i;
        }

        idles.insert(idles.end(), pendingIdle.begin(), pendingIdle.end());
        chasings.insert(chasings.end(), pendingChase.begin(), pendingChase.end());
        attacking.insert(attacking.end(), pendingAttack.begin(), pendingAttack.end());
    }
};

} // namespace FSMTableState