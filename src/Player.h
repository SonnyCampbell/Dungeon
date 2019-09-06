#pragma once
#include "Game Components/Game.h"
#include "AnimatedSprite.h"
#include "Vec2.h"
#include "RigidBody.h"
#include "Weapon.h"
#include "AttackTypes.h"

struct Player
{
    int id = 0;
    AnimatedSprite *sprite;
    RigidBody rb;
    Weapon *weapon;
};

namespace PlayerManager
{
Player NewPlayer(SDL_Renderer **renderer, Vec2 position, float speed = 166.0f);
void UpdatePlayer(Player &player);
void DrawPlayer(Player &player);
void PlayerHandleInputEvent(const SDL_Event &event, Player &player);
void PlayerAttack(Player player, AttackTypes attack_type);
void UpdatePlayerDirection(Player &player, Vec2 movement_vector);
void DrawPlayerDebugRect(Player &player);
} // namespace PlayerManager
