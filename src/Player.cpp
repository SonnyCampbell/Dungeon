#pragma once
#include "Player.h"

using namespace AnimatedSpriteManager;
using namespace WeaponManager;

namespace PlayerManager
{

Player NewPlayer(SDL_Renderer **renderer, Vec2 position, float speed)
{
    Vec2 frameSize = {16, 20}; //TODO Constants - Spritesheet frame width/height ---- ^ also --16:28 is default
    int runFps = 10;
    int idleFps = 5;
    auto *animations = new std::map<AnimationKey, Animation *>({{IdleUp, new Animation(4, idleFps, frameSize, {128, 44})},
                                                                {IdleDown, new Animation(4, idleFps, frameSize, {128, 36})},
                                                                {IdleLeft, new Animation(4, idleFps, frameSize, {128, 36})},
                                                                {IdleRight, new Animation(4, idleFps, frameSize, {128, 36})},
                                                                {WalkUp, new Animation(4, runFps, frameSize, {192, 44})},
                                                                {WalkDown, new Animation(4, runFps, frameSize, {192, 36})},
                                                                {WalkLeft, new Animation(4, runFps, frameSize, {192, 36})},
                                                                {WalkRight, new Animation(4, runFps, frameSize, {192, 36})}});

    auto rb = RigidBody(60.f, 16.f, 20.f, position, speed, Vec2(0, 0));

    auto sprite = NewAnimatedSprite(renderer, "assets/DungeonTilesetV2.png", animations, WalkUp);

    auto weapon = createHeavyMace(renderer, rb.aabb.center, 0);

    Player player = {0, sprite, rb, weapon};

    return player;
}

void UpdatePlayer(Player &player)
{
    Vec2 currentDirection = Vec2(player.rb.direction.x(), player.rb.direction.y());

    if (currentDirection.x() == 0 && currentDirection.y() == 0)
    {
        if (player.sprite->currentAnimationKey != IdleUp)
        {
            ResetAnimation(*player.sprite, IdleUp);
            if (player.weapon)
                ResetFrames(*player.weapon);
        }
    }
    else
    {
        if (player.sprite->currentAnimationKey != WalkUp)
        {
            ResetAnimation(*player.sprite, WalkUp);
            if (player.weapon)
                ResetFrames(*player.weapon);
        }
    }

    UpdateAnimation(*player.sprite, player.sprite->currentAnimationKey);
    if (player.weapon)
    {
        UpdateWeapon(*player.weapon, player.rb.aabb.center);
    }
}

void DrawPlayer(Player &player, SDL_Rect &camera)
{
    auto camera_position = Vec2(camera.x, camera.y);
    DrawSprite(*player.sprite, player.rb.aabb.min() - camera_position);
    if (player.weapon)
        DrawWeapon(*player.weapon, camera_position, CurrentAnimation(*player.sprite)->currentFrameCount(), player.sprite->facingRight);
}

void DrawPlayerDebugRect(Player &player, SDL_Rect &camera)
{
    //DEBUG DRAWING
    SDL_FRect debug_rect = {player.rb.aabb.min().x() - camera.x, player.rb.aabb.min().y() - camera.y, (float)CurrentAnimation(*player.sprite)->CurrentFrame().w, (float)CurrentAnimation(*player.sprite)->CurrentFrame().h};
    SDL_SetRenderDrawColor(*player.sprite->texture.gRenderer, 0x00, 0x00, 0xFF, 0xFF);
    SDL_RenderDrawRectF(*player.sprite->texture.gRenderer, &debug_rect);
    SDL_SetRenderDrawColor(*player.sprite->texture.gRenderer, 0xFF, 0x00, 0x00, 0xFF);
}

void PlayerHandleInputEvent(const SDL_Event &event, Player &player)
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
    if (player.weapon)
        WeaponAttack(*player.weapon, attack_type);
}
void UpdatePlayerDirection(Player &player, Vec2 movement_vector)
{
    player.rb.direction += movement_vector;
}
} // namespace PlayerManager
