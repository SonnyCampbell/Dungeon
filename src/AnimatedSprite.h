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
    int entity_id;
    LTexture texture;
    std::map<AnimationKey, Animation *> *animations;
    AnimationKey currentAnimationKey;
    bool isAnimating;
    bool facingRight;
};

namespace AnimatedSpriteManager
{
AnimatedSprite *NewAnimatedSprite(SDL_Renderer **renderer, int entity_id, std::string texturePath, std::map<AnimationKey, Animation *> *animations, AnimationKey animationKey);

void DeleteAnimatedSprite(AnimatedSprite &sprite);

Animation *CurrentAnimation(const AnimatedSprite &sprite);

Vec2 Size(const AnimatedSprite &sprite);

void ResetAnimation(AnimatedSprite &sprite, AnimationKey key);

Animation *UpdateAnimation(AnimatedSprite &sprite, AnimationKey key);

void DrawSprite(AnimatedSprite &sprite, Vec2 position);

}; // namespace AnimatedSpriteManager
