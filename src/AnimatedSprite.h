#pragma once
#include <map>
#include <SDL.h>
#include "LTexture.h"
#include "AnimationKey.h"
#include "Animation.h"

class AnimatedSprite
{
private:
    /* data */
public:
    LTexture texture;
    std::map<AnimationKey, Animation *> animations;
    AnimationKey currentAnimationKey;
    SDL_Point position;
    bool isAnimating;
    float speed;
    bool facingRight;

    AnimatedSprite(SDL_Renderer **renderer, std::string texturePath, std::map<AnimationKey, Animation *> _animations, AnimationKey _animationKey, SDL_Point _position, float speed = 166.0f) : texture(renderer)
    {
        if (!texture.loadFromFile(texturePath))
        {
            printf("fuck");
        }
        animations = _animations;
        position = _position;
        currentAnimationKey = _animationKey;
        isAnimating = true;
        speed = 166.0f;
        facingRight = true;
    }

    ~AnimatedSprite()
    {
    }

    Animation *CurrentAnimation()
    {
        return animations[currentAnimationKey];
    }

    SDL_Point Size()
    {
        return CurrentAnimation()->size;
    }

    void ResetAnimation(AnimationKey key)
    {
        animations[key]->Reset();
    }

    Animation *UpdateAnimation(AnimationKey key, double elapsedGameTime)
    {
        if (isAnimating)
        {
            return animations[key]->Update(elapsedGameTime);
        }
        else
        {
            return animations[key];
        }
    }

    void Draw()
    {
        SDL_Rect *currentClip = &CurrentAnimation()->CurrentFrame();

        texture.render((800 - currentClip->w - 50) / 2, (600 - currentClip->h) / 2, currentClip, NULL, NULL, SDL_FLIP_NONE);
    }
};
