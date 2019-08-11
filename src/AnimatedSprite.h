#pragma once
#include <map>
#include <SDL.h>
#include <stdio.h>
#include <iostream>

#include "LTexture.h"
#include "AnimationKey.h"
#include "Animation.h"

class AnimatedSprite
{
private:
    /* data */
public:
    LTexture texture;
    std::map<AnimationKey, Animation *> *animations;
    AnimationKey currentAnimationKey;
    bool isAnimating;
    bool facingRight;

    AnimatedSprite(SDL_Renderer **renderer, std::string texturePath, std::map<AnimationKey, Animation *> *_animations, AnimationKey _animationKey) : texture(renderer)
    {
        if (!texture.loadFromFile(texturePath))
        {
            printf("fuck");
        }
        animations = _animations;
        currentAnimationKey = _animationKey;
        isAnimating = true;
        facingRight = true;
    }

    ~AnimatedSprite()
    {
        delete animations;
    }

    Animation *CurrentAnimation()
    {
        return animations->at(currentAnimationKey);
    }

    SDL_Point Size()
    {
        return CurrentAnimation()->size;
    }

    void ResetAnimation(AnimationKey key)
    {
        animations->at(key)->Reset();
    }

    Animation *UpdateAnimation(AnimationKey key, double elapsedGameTime)
    {
        if (isAnimating)
        {
            return animations->at(key)->Update(elapsedGameTime);
        }
        else
        {
            return animations->at(key);
        }
    }

    void Draw(SDL_Point position)
    {
        SDL_Rect currentClip = CurrentAnimation()->CurrentFrame();

        texture.render(position.x, position.y, &currentClip, NULL, NULL, SDL_FLIP_NONE);
    }
};
