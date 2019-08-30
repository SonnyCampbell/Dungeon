#pragma once
#include "RigidBody.h"
#include "AnimatedSprite.h"
#include "AttackTypes.h"

struct AttackFrame
{
    float angle;
    Vec2 attack_offset;
};

struct Weapon
{
    LTexture texture;
    RigidBody rb;
    Vec2 offset;
    SDL_Rect spritesheet_clip;
    int total_current_frame = 4;
    int current_frame = 0;
    int direction = 0;

    bool isAttacking = false;
    AttackTypes attack_type;
    double attack_time = 500; //ms
    double last_frame_time = 0;
    int current_attack_frame = 0;
    AttackFrame basic_attack_framse[4] =
        {{20.f, Vec2(0, -8)},
         {120.f, Vec2(7, 7)},
         {160.f, Vec2(0, 10)},
         {190.f, Vec2(-10, 10)}};
    float frame_length = 16.6f;
};

namespace WeaponManager
{
Weapon *NewWeapon(SDL_Renderer **renderer, SDL_Rect clip, Vec2 position, Vec2 offset, int current_frame);
void DeleteWeapon(Weapon &weapon);
void DrawWeapon(Weapon &weapon, Vec2 position, int frame, bool facingRight);
void ResetFrames(Weapon &weapon);
void WeaponAttack(Weapon &weapon, AttackTypes attack_type);
void UpdateWeapon(Weapon &weapon, double elapsed_game_time);
Weapon *createSword(SDL_Renderer **renderer, Vec2 position, int current_frame);
Weapon *createNailBoard(SDL_Renderer **renderer, Vec2 position, int current_frame);
Weapon *createHeavyMace(SDL_Renderer **renderer, Vec2 position, int current_frame);
}; // namespace WeaponManager
