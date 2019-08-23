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
};

Weapon::Weapon(SDL_Renderer **renderer, SDL_Rect clip, Vec2 position, Vec2 offset, int current_frame_count)
    : texture(renderer), rb(0.f, clip.w, clip.h, position, 0.f, Vec2::zero()), offset(offset)
{
    //Don't load this from file every time - copy existing spritesheet texture
    if (!texture.loadFromFile("assets/DungeonTilesetV2.png"))
    {
        printf("fuck");
    }

    spritesheet_clip = clip;
    current_frame = current_frame_count;
}

Weapon::~Weapon()
{
}

RigidBody Weapon::getRigidBody()
{
    return rb;
}

void Weapon::ResetFrames()
{
    current_frame = 0;
    direction = 0;
}

void Weapon::Attack(AttackTypes new_attack_type)
{
    printf("Attack!\n");
    if (!isAttacking)
    {
        isAttacking = true;
        attack_type = new_attack_type;
    }
}

void Weapon::Update(double elapsed_game_time)
{
    if (isAttacking)
    {
        if (elapsed_game_time - last_frame_time > frame_length)
        {

            last_frame_time = elapsed_game_time;
            auto next_frame = current_attack_frame + 1;
            if (next_frame >= 4)
            {
                isAttacking = false;
                current_attack_frame = 0;
                ResetFrames();
                return;
            }
            current_attack_frame = (next_frame) % 4;
        }
    }
}

void Weapon::Draw(Vec2 position, int frame, bool facingRight)
{
    if (!isAttacking)
    {
        if (current_frame != frame)
        {
            direction += (frame == 1 || frame == 2) ? 1 : -1;
        }

        if (facingRight)
        {
            texture.renderF(position.x(), position.y() - offset.y() + direction, &spritesheet_clip, 90.f, NULL, SDL_FLIP_NONE);
        }
        else
        {
            texture.renderF(position.x() - offset.x(), position.y() - offset.y() + direction + 1, &spritesheet_clip, -90.f, NULL, SDL_FLIP_NONE);
        }

        current_frame = frame;
    }
    else
    {
        auto attack = basic_attack_framse[current_attack_frame];
        if (facingRight)
        {
            texture.renderF(position.x() + attack.attack_offset.x(), position.y() - offset.y() + attack.attack_offset.y(), &spritesheet_clip, attack.angle, NULL, SDL_FLIP_NONE);
        }
        else
        {
            texture.renderF(position.x() - offset.x() - attack.attack_offset.x(), position.y() - offset.y() + attack.attack_offset.y() + 1, &spritesheet_clip, -attack.angle, NULL, SDL_FLIP_NONE);
        }
    }
}

Weapon *createSword(SDL_Renderer **renderer, Vec2 position, int current_frame)
{
    return new Weapon(renderer, {322, 25, 11, 22}, position, Vec2(11.f, 7.f), current_frame);
}
