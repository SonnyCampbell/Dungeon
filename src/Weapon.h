#pragma once
#include "RigidBody.h"
#include "AnimatedSprite.h"
#include "AttackTypes.h"

struct AttackFrame
{
    float angle;
    Vec2 attack_offset;
};

class Weapon
{
private:
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

public:
    Weapon(SDL_Renderer **renderer, SDL_Rect clip, Vec2 position, Vec2 offset, int current_frame);
    ~Weapon();
    RigidBody getRigidBody();
    void Draw(Vec2 position, int frame, bool facingRight);
    void ResetFrames();
    void Attack(AttackTypes attack_type);
    void Update(double elapsed_game_time);
    static Weapon *createSword(SDL_Renderer **renderer, Vec2 position, int current_frame);
};