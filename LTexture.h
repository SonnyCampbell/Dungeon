#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <string>

//Texture wrapper class
class LTexture
{
public:
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

    //Loads image at specified path
    bool loadFromFile(std::string path);

    //Renders texture at given point
    void render(int x, int y, SDL_Rect *clip = NULL);

    //Set color modulation
    void setColor(Uint8 red, Uint8 green, Uint8 blue);

    //Set blending
    void setBlendMode(SDL_BlendMode blending);

    //Set alpha modulation
    void setAlpha(Uint8 alpha);

private:
    // The Global Renderer
    SDL_Renderer **gRenderer;

    //The actual hardware texture
    SDL_Texture *mTexture;

    //Image dimensions
    int mWidth;
    int mHeight;
};

void LTexture::render(int x, int y, SDL_Rect *clip)
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = {x, y, mWidth, mHeight};

    //Set clip rendering dimensions
    if (clip != NULL)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    //Render to screen
    SDL_RenderCopy(*gRenderer, mTexture, clip, &renderQuad);
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
    //Modulate texture
    SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending)
{
    //Set blending function
    SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha)
{
    //Modulate texture alpha
    SDL_SetTextureAlphaMod(mTexture, alpha);
}

bool LTexture::loadFromFile(std::string path)
{
    //Get rid of preexisting texture
    free();

    //The final texture
    SDL_Texture *newTexture = NULL;

    //Load image at specified path
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
        return false;
    }

    //Color key image
    SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
    //Create texture from surface pixels
    newTexture = SDL_CreateTextureFromSurface(*gRenderer, loadedSurface);
    if (newTexture == NULL)
    {
        printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
    }
    else
    {
        //Get image dimensions
        mWidth = loadedSurface->w;
        mHeight = loadedSurface->h;
    }

    //Get rid of old loaded surface
    SDL_FreeSurface(loadedSurface);

    //Return success
    mTexture = newTexture;
    return mTexture != NULL;
}