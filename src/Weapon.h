#pragma once
#include "RigidBody.h"
#include "AnimatedSprite.h"

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

public:
    Weapon(SDL_Renderer **renderer, SDL_Rect clip, Vec2 position, Vec2 offset, int current_frame);
    ~Weapon();
    RigidBody getRigidBody();
    void Draw(Vec2 position, int frame, bool facingRight);
    void ResetFrames();
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

void Weapon::Draw(Vec2 position, int frame, bool facingRight)
{
    if (current_frame != frame)
    {
        direction += (frame == 1 || frame == 2) ? 1 : -1;
    }

    printf("Frame %d - Current_Frame %d \n", frame, current_frame);
    printf("Direction %d \n", direction);

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

Weapon *createSword(SDL_Renderer **renderer, Vec2 position, int current_frame)
{
    return new Weapon(renderer, {322, 25, 11, 22}, position, Vec2(11.f, 7.f), current_frame);
}
