#pragma once
#include "AnimatedSprite.h"
#include "Vec2.h"
#include "RigidBody.h"
#include "Weapon.h"
#include "AttackTypes.h"

class Player
{
private:
public:
    AnimatedSprite *sprite;
    RigidBody rb;

    // If I change this to [Weapon weapon] and assign as
    // weapon = Weapon(renderer) in the constructor,
    // why doesn't weapon.Draw() work???
    Weapon *weapon;

    Player(SDL_Renderer **renderer, Vec2 position, float speed = 166.0f) : rb(60.f, 16.f, 20.f, position, speed, Vec2(0, 0))
    {
        Vec2 frameSize = {16, 20}; //TODO Constants - Spritesheet frame width/height ---- ^ also --16:28 is default
        int runFps = 10;
        int idleFps = 5;
        std::map<AnimationKey, Animation *> *animations = new std::map<AnimationKey, Animation *>({{IdleUp, new Animation(4, idleFps, frameSize, {128, 44})},
                                                                                                   {IdleDown, new Animation(4, idleFps, frameSize, {128, 36})},
                                                                                                   {IdleLeft, new Animation(4, idleFps, frameSize, {128, 36})},
                                                                                                   {IdleRight, new Animation(4, idleFps, frameSize, {128, 36})},
                                                                                                   {WalkUp, new Animation(4, runFps, frameSize, {192, 44})},
                                                                                                   {WalkDown, new Animation(4, runFps, frameSize, {192, 36})},
                                                                                                   {WalkLeft, new Animation(4, runFps, frameSize, {192, 36})},
                                                                                                   {WalkRight, new Animation(4, runFps, frameSize, {192, 36})}});

        sprite = new AnimatedSprite(renderer, "assets/DungeonTilesetV2.png", animations, WalkUp);
        weapon = createSword(renderer, rb.aabb.center, 0);
    }

    ~Player()
    {
        delete sprite;
        delete weapon;
    }

    void Draw();
    void Update(double currentTick, float dt);
    //void MovePlayer(Vec2 movement_vector, float dt);
    void HandleInputEvent(const SDL_Event &event, float dt);
    void UpdateDirection(Vec2 movement_vector);
    void Attack(AttackTypes attack_type);
};

void Player::Update(double currentTick, float dt)
{
    Vec2 currentDirection = Vec2(rb.direction.x(), rb.direction.y());

    if (currentDirection.x() == 0 && currentDirection.y() == 0)
    {
        if (sprite->currentAnimationKey != IdleUp)
        {
            sprite->ResetAnimation(IdleUp);
            weapon->ResetFrames();
        }
    }
    else
    {
        if (sprite->currentAnimationKey != WalkUp)
        {
            sprite->ResetAnimation(WalkUp);
            weapon->ResetFrames();
        }
    }

    sprite->UpdateAnimation(sprite->currentAnimationKey, currentTick);
    if (weapon != nullptr)
    {
        weapon->Update(currentTick);
    }
}

void Player::Draw()
{
    sprite->Draw(rb.aabb.min());
    weapon->Draw(rb.aabb.center, sprite->CurrentAnimation()->currentFrameCount(), sprite->facingRight);

    //DEBUG DRAWING
    // SDL_FRect debug_rect = {rb.aabb.min().x(), rb.aabb.min().y(), (float)sprite->CurrentAnimation()->CurrentFrame().w, (float)sprite->CurrentAnimation()->CurrentFrame().h};
    // SDL_SetRenderDrawColor(*sprite->texture.gRenderer, 0x00, 0x00, 0xFF, 0xFF);
    // SDL_RenderDrawRectF(*sprite->texture.gRenderer, &debug_rect);
    // SDL_SetRenderDrawColor(*sprite->texture.gRenderer, 0xFF, 0x00, 0x00, 0xFF);
    // SDL_RenderDrawPointF(*sprite->texture.gRenderer, rb.aabb.center.x(), rb.aabb.center.y());
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
        case SDLK_SPACE:
            Attack(AttackTypes::BasicAttack);
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

void Player::Attack(AttackTypes attack_type)
{
    weapon->Attack(attack_type);
}

void Player::UpdateDirection(Vec2 movement_vector)
{
    rb.direction += movement_vector;
}
