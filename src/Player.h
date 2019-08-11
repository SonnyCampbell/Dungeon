#pragma once
#include "AnimatedSprite.h"

class Player
{
private:
public:
    AnimatedSprite *sprite;
    SDL_Point position;
    float speed;

    Player(SDL_Renderer **renderer, SDL_Point _position, float _speed = 166.0f)
    {
        position = _position;
        speed = _speed;

        SDL_Point frameSize = {16, 28};
        std::map<AnimationKey, Animation *> *animations = new std::map<AnimationKey, Animation *>({{IdleUp, new Animation(4, 4, frameSize, {128, 36})},
                                                                                                   {IdleDown, new Animation(4, 4, frameSize, {128, 36})},
                                                                                                   {IdleLeft, new Animation(4, 4, frameSize, {128, 36})},
                                                                                                   {IdleRight, new Animation(4, 4, frameSize, {128, 36})},
                                                                                                   {WalkUp, new Animation(4, 4, frameSize, {192, 36})},
                                                                                                   {WalkDown, new Animation(4, 4, frameSize, {192, 36})},
                                                                                                   {WalkLeft, new Animation(4, 4, frameSize, {192, 36})},
                                                                                                   {WalkRight, new Animation(4, 4, frameSize, {192, 36})}});

        sprite = new AnimatedSprite(renderer, "assets/DungeonTilesetV2.png", animations, IdleUp);
    }

    ~Player()
    {
        delete sprite;
    }

    void Draw();
    void Update();
};

void Player::Update()
{
    sprite->UpdateAnimation(sprite->currentAnimationKey, SDL_GetTicks());
}

void Player::Draw()
{
    sprite->Draw(position);
}
