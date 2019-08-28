#pragma once
#include <map>
#include <SDL.h>
#include <stdio.h>
#include <iostream>
#include "LTexture.h"
#include "AnimationKey.h"
#include "Animation.h"
#include "Vec2.h"

struct AnimatedSprite
{
    LTexture texture;
    std::map<AnimationKey, Animation *> *animations;
    AnimationKey currentAnimationKey;
    bool isAnimating;
    bool facingRight;
};

namespace AnimatedSpriteManager
{
AnimatedSprite *NewAnimatedSprite(SDL_Renderer **renderer, std::string texturePath, std::map<AnimationKey, Animation *> *animations, AnimationKey animationKey);

void DeleteAnimatedSprite(AnimatedSprite &sprite);

Animation *CurrentAnimation(const AnimatedSprite &sprite);

Vec2 Size(const AnimatedSprite &sprite);

void ResetAnimation(AnimatedSprite &sprite, AnimationKey key);

Animation *UpdateAnimation(AnimatedSprite &sprite, AnimationKey key, double elapsedGameTime);

void DrawSprite(AnimatedSprite &sprite, Vec2 position);

}; // namespace AnimatedSpriteManager
