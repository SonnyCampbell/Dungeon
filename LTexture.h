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
        freeTexture();
        free(gRenderer);
    }

    //Deallocates texture
    void freeTexture()
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
    void render(int x, int y);

private:
    // The Global Renderer
    SDL_Renderer **gRenderer;

    //The actual hardware texture
    SDL_Texture *mTexture;

    //Image dimensions
    int mWidth;
    int mHeight;
};

void LTexture::render(int x, int y)
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = {x, y, mWidth, mHeight};
    SDL_RenderCopy(*gRenderer, mTexture, NULL, &renderQuad);
}

bool LTexture::loadFromFile(std::string path)
{
    //Get rid of preexisting texture
    freeTexture();

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