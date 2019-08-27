#pragma once
#include "Player.h"

namespace Thing
{

Player NewPlayer(SDL_Renderer **renderer, Vec2 position, float speed)
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

    auto rb = RigidBody(60.f, 16.f, 20.f, position, speed, Vec2(0, 0));

    auto sprite = new AnimatedSprite(renderer, "assets/DungeonTilesetV2.png", animations, WalkUp);
    auto weapon = Weapon::createSword(renderer, rb.aabb.center, 0);

    Player player = {sprite, rb, weapon};

    return player;
}

void UpdatePlayer(Player &player, double currentTick, float dt)
{
    Vec2 currentDirection = Vec2(player.rb.direction.x(), player.rb.direction.y());

    if (currentDirection.x() == 0 && currentDirection.y() == 0)
    {
        if (player.sprite->currentAnimationKey != IdleUp)
        {
            player.sprite->ResetAnimation(IdleUp);
            player.weapon->ResetFrames();
        }
    }
    else
    {
        if (player.sprite->currentAnimationKey != WalkUp)
        {
            player.sprite->ResetAnimation(WalkUp);
            player.weapon->ResetFrames();
        }
    }

    player.sprite->UpdateAnimation(player.sprite->currentAnimationKey, currentTick);
    if (player.weapon)
    {
        player.weapon->Update(currentTick);
    }
}

void DrawPlayer(Player &player)
{
    player.sprite->Draw(player.rb.aabb.min());
    player.weapon->Draw(player.rb.aabb.center, player.sprite->CurrentAnimation()->currentFrameCount(), player.sprite->facingRight);

    //DEBUG DRAWING
    // SDL_FRect debug_rect = {rb.aabb.min().x(), rb.aabb.min().y(), (float)sprite->CurrentAnimation()->CurrentFrame().w, (float)sprite->CurrentAnimation()->CurrentFrame().h};
    // SDL_SetRenderDrawColor(*sprite->texture.gRenderer, 0x00, 0x00, 0xFF, 0xFF);
    // SDL_RenderDrawRectF(*sprite->texture.gRenderer, &debug_rect);
    // SDL_SetRenderDrawColor(*sprite->texture.gRenderer, 0xFF, 0x00, 0x00, 0xFF);
    // SDL_RenderDrawPointF(*sprite->texture.gRenderer, rb.aabb.center.x(), rb.aabb.center.y());
}

void PlayerHandleInputEvent(const SDL_Event &event, Player &player, float dt)
{
    if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
    {
        switch (event.key.keysym.sym)
        {
        case SDLK_w:
            UpdatePlayerDirection(player, Vec2(0, -1));
            break;
        case SDLK_s:
            UpdatePlayerDirection(player, Vec2(0, 1));
            break;
        case SDLK_a:
            player.sprite->facingRight = false;
            UpdatePlayerDirection(player, Vec2(-1, 0));
            break;
        case SDLK_d:
            player.sprite->facingRight = true; //Maybe put this in update and do it based on direction vector to prevent moonwalking. Looks fun to moonwalk though.
            UpdatePlayerDirection(player, Vec2(1, 0));
            break;
        case SDLK_SPACE:
            PlayerAttack(player, AttackTypes::BasicAttack);
            break;
        }
    }
    else if (event.type == SDL_KEYUP && event.key.repeat == 0)
    {
        switch (event.key.keysym.sym)
        {
        case SDLK_w:
            UpdatePlayerDirection(player, Vec2(0, 1));
            break;
        case SDLK_s:
            UpdatePlayerDirection(player, Vec2(0, -1));
            break;
        case SDLK_a:
            UpdatePlayerDirection(player, Vec2(1, 0));
            break;
        case SDLK_d:
            UpdatePlayerDirection(player, Vec2(-1, 0));
            break;
        }
    }
}

void PlayerAttack(Player player, AttackTypes attack_type)
{
    if (player.weapon != nullptr)
    {
        player.weapon->Attack(attack_type);
    }
}
void UpdatePlayerDirection(Player &player, Vec2 movement_vector)
{
    player.rb.direction += movement_vector;
}
} // namespace Thing
