#pragma once
#include "AnimatedSprite.h"

namespace AnimatedSpriteManager
{

AnimatedSprite *NewAnimatedSprite(SDL_Renderer **renderer, int id, std::string texturePath, std::map<AnimationKey, Animation *> *animations, AnimationKey animationKey)
{
    auto texture = new LTexture(renderer);
    if (!texture->loadFromFile(texturePath))
    {
        printf("fuck");
    }

    return new AnimatedSprite{id, *texture, animations, animationKey, true, true};
}

void DeleteAnimatedSprite(AnimatedSprite &sprite)
{
    delete sprite.animations;
    delete &sprite;
}

Animation *CurrentAnimation(const AnimatedSprite &sprite)
{
    return sprite.animations->at(sprite.currentAnimationKey);
}

Vec2 Size(const AnimatedSprite &sprite)
{
    return CurrentAnimation(sprite)->size;
}

void ResetAnimation(AnimatedSprite &sprite, AnimationKey key)
{
    sprite.currentAnimationKey = key;
    sprite.animations->at(key)->Reset();
}

Animation *UpdateAnimation(AnimatedSprite &sprite, AnimationKey key)
{
    if (sprite.isAnimating)
    {
        return sprite.animations->at(key)->Update();
    }
    else
    {
        return sprite.animations->at(key);
    }
}

void DrawSprite(AnimatedSprite &sprite, Vec2 position)
{
    SDL_Rect currentClip = CurrentAnimation(sprite)->CurrentFrame();

    if (sprite.facingRight)
    {
        sprite.texture.renderF(position.x(), position.y(), &currentClip, NULL, NULL, SDL_FLIP_NONE);
    }
    else
    {
        sprite.texture.renderF(position.x(), position.y(), &currentClip, NULL, NULL, SDL_FLIP_HORIZONTAL);
    }
}
} // namespace AnimatedSpriteManager