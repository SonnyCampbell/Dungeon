#pragma once
#include <SDL.h>

#include "Vec2.h"

class Animation
{
private:
public:
    SDL_Rect *frames;
    int framesCount;
    int fps = 60;
    int currentFrame;
    double lastFrameTime;
    double frameLength;
    Vec2 size;

    Animation(int _framesCount, int _fps, Vec2 _size, Vec2 offset)
    {
        //frames = aFrames;
        size = _size;
        framesCount = _framesCount;
        frames = new SDL_Rect[framesCount];

        for (int i = 0; i < framesCount; i++)
        {
            frames[i] = {(int)(offset.x() + (size.x() * i)), (int)offset.y(), (int)size.x(), (int)size.y()};
        }

        fps = _fps;
        currentFrame = 0;
        lastFrameTime = 0;
        frameLength = 1000 * (1.0f / fps);
    }
    ~Animation()
    {
        delete frames;
    }

    SDL_Rect CurrentFrame()
    {
        if (currentFrame >= framesCount)
        {
            return {0, 0, 0, 0};
        }
        return frames[currentFrame];
    }

    void Reset()
    {
        currentFrame = 0;
        lastFrameTime = 0;
    }

    Animation *Update(double elapsedGameTime)
    {
        //double currentTime = lastFrameTime + elapsedGameTime;
        if (elapsedGameTime - lastFrameTime > frameLength)
        {
            lastFrameTime = elapsedGameTime;
            currentFrame = (currentFrame + 1) % framesCount;
        }

        return this;
    }
};
