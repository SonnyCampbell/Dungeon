#pragma once
#include <SDL.h>
#include <chrono>

class Animation
{
private:
public:
    SDL_Rect frames[4];
    int framesCount;
    int fps = 60;
    int currentFrame;
    double frameTimer;
    double frameLength;
    SDL_Point size;

    Animation(int aFramesCount, int aFps, SDL_Point aSize, SDL_Point offset)
    {
        //frames = aFrames;
        size = aSize;
        framesCount = aFramesCount;
        //frames = new SDL_Rect[framesCount];

        for (int i = 0; i < framesCount; i++)
        {
            frames[i] = {offset.x + (size.x * i), offset.y, size.x, size.y};
        }

        fps = aFps;
        currentFrame = 0;
        frameTimer = 0;
        frameLength = 1000 * (1.0f / fps);
    }
    ~Animation()
    {
        //delete frames;
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
        frameTimer = 0;
    }

    Animation *Update(double elapsedGameTime)
    {
        double currentTime = frameTimer + elapsedGameTime;
        if (currentTime > frameLength)
        {
            frameTimer = 0;
            currentFrame = (currentFrame + 1) % framesCount;
        }
        else
        {
            frameTimer = currentTime;
            currentFrame = currentFrame;
        }
        return this;
    }
};
