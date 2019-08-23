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
    int current_frame;
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
        current_frame = 0;
        lastFrameTime = 0;
        frameLength = 1000 * (1.0f / fps);
    }
    ~Animation()
    {
        delete frames;
    }

    SDL_Rect CurrentFrame()
    {
        if (current_frame >= framesCount)
        {
            return {0, 0, 0, 0};
        }
        return frames[current_frame];
    }

    int currentFrameCount()
    {
        return current_frame;
    }

    void Reset()
    {
        current_frame = 0;
        lastFrameTime = 0;
    }

    Animation *Update(double elapsedGameTime)
    {
        //double currentTime = lastFrameTime + elapsedGameTime;
        if (elapsedGameTime - lastFrameTime > frameLength)
        {
            lastFrameTime = elapsedGameTime;
            current_frame = (current_frame + 1) % framesCount;
        }

        return this;
    }
};
