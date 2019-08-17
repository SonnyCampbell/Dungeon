#pragma once
#include "AnimatedSprite.h"
#include "Vec2.h"
#include "RigidBody.h"

class Player
{
private:
public:
    AnimatedSprite *sprite;
    RigidBody rb;

    Player(SDL_Renderer **renderer, Vec2 position, float speed = 166.0f) : rb(60.f, 16.f, 28.f, position, speed, Vec2(0, 0))
    {
        Vec2 frameSize = {16, 28}; //TODO Constants - Spritesheet frame width/height ---- ^ also
        int runFps = 10;
        int idleFps = 5;
        std::map<AnimationKey, Animation *> *animations = new std::map<AnimationKey, Animation *>({{IdleUp, new Animation(4, idleFps, frameSize, {128, 36})},
                                                                                                   {IdleDown, new Animation(4, idleFps, frameSize, {128, 36})},
                                                                                                   {IdleLeft, new Animation(4, idleFps, frameSize, {128, 36})},
                                                                                                   {IdleRight, new Animation(4, idleFps, frameSize, {128, 36})},
                                                                                                   {WalkUp, new Animation(4, runFps, frameSize, {192, 36})},
                                                                                                   {WalkDown, new Animation(4, runFps, frameSize, {192, 36})},
                                                                                                   {WalkLeft, new Animation(4, runFps, frameSize, {192, 36})},
                                                                                                   {WalkRight, new Animation(4, runFps, frameSize, {192, 36})}});

        sprite = new AnimatedSprite(renderer, "assets/DungeonTilesetV2.png", animations, WalkUp);
    }

    ~Player()
    {
        delete sprite;
    }

    void Draw();
    void Update(double currentTick, float dt);
    //void MovePlayer(Vec2 movementVector, float dt);
    void HandleInputEvent(const SDL_Event &event, float dt);
    void UpdateDirection(Vec2 movementVector);
};

void Player::Update(double currentTick, float dt)
{
    Vec2 currentDirection = Vec2(rb.direction.x(), rb.direction.y());

    if (currentDirection.x() == 0 && currentDirection.y() == 0)
    {
        if (sprite->currentAnimationKey != IdleUp)
        {
            sprite->ResetAnimation(IdleUp);
        }
    }
    else
    {
        currentDirection.normalize();
        rb.aabb.center = rb.aabb.center + (currentDirection * rb.speed * dt);

        if (sprite->currentAnimationKey != WalkUp)
        {
            sprite->ResetAnimation(WalkUp);
        }
    }

    sprite->UpdateAnimation(sprite->currentAnimationKey, currentTick);
}

void Player::Draw()
{
    sprite->Draw(rb.aabb.center);
}

void Player::HandleInputEvent(const SDL_Event &event, float dt)
{
    if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
    {
        switch (event.key.keysym.sym)
        {
        case SDLK_w:
            UpdateDirection(Vec2(0, -1));
            break;
        case SDLK_s:
            UpdateDirection(Vec2(0, 1));
            break;
        case SDLK_a:
            sprite->facingRight = false;
            UpdateDirection(Vec2(-1, 0));
            break;
        case SDLK_d:
            sprite->facingRight = true; //Maybe put this in update and do it based on direction vector to prevent moonwalking. Looks fun to moonwalk though.
            UpdateDirection(Vec2(1, 0));
            break;
        }
    }
    else if (event.type == SDL_KEYUP && event.key.repeat == 0)
    {
        switch (event.key.keysym.sym)
        {
        case SDLK_w:
            UpdateDirection(Vec2(0, 1));
            break;
        case SDLK_s:
            UpdateDirection(Vec2(0, -1));
            break;
        case SDLK_a:
            UpdateDirection(Vec2(1, 0));
            break;
        case SDLK_d:
            UpdateDirection(Vec2(-1, 0));
            break;
        }
    }
}

void Player::UpdateDirection(Vec2 movementVector)
{
    rb.direction += movementVector;
}
