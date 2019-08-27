#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <string>

//Texture wrapper class
class LTexture
{
public:
    // The Global Renderer
    SDL_Renderer **gRenderer;

    //Initializes variables
    LTexture(SDL_Renderer **renderer)
    {
        //Initialize
        gRenderer = renderer;
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }

    //Deallocates memory
    ~LTexture()
    {
        free();
    }

    //Deallocates texture
    void free()
    {
        //Free texture if it exists
        if (mTexture != NULL)
        {
            SDL_DestroyTexture(mTexture);
            mTexture = NULL;
            mWidth = 0;
            mHeight = 0;
        }
    }

    //Gets image dimensions
    int getWidth()
    {
        return mWidth;
    }
    int getHeight()
    {
        return mHeight;
    }

    SDL_Texture *getTexture()
    {
        return mTexture;
    }

    //Loads image at specified path
    bool loadFromFile(std::string path);

    //Renders texture at given point
    void render(int x, int y, SDL_Rect *clip = NULL, double angle = 0.0, SDL_Point *center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
    void renderF(float x, float y, SDL_Rect *clip, double angle, SDL_FPoint *center, SDL_RendererFlip flip);

    //Set color modulation
    void setColor(Uint8 red, Uint8 green, Uint8 blue);

    //Set blending
    void setBlendMode(SDL_BlendMode blending);

    //Set alpha modulation
    void setAlpha(Uint8 alpha);

private:
    //The actual hardware texture
    SDL_Texture *mTexture;

    //Image dimensions
    int mWidth;
    int mHeight;
};

