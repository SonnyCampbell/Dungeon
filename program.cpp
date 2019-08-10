#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

#include "LTexture.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

enum KeyPressSurfaces
{
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};

// Global Window
SDL_Window *gWindow = NULL;

//The window renderer
SDL_Renderer *gRenderer = NULL;

//Scene textures
LTexture gFooTexture(&gRenderer);
LTexture gBackgroundTexture(&gRenderer);

//Current displayed texture
SDL_Texture *gTexture = NULL;

//The surface contained by the window
SDL_Surface *gScreenSurface = NULL;

bool init()
{
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    //The window we'll be rendering to
    gWindow = SDL_CreateWindow("SDL2 Pixel Drawing", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (gWindow == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    //Create renderer for window
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (gRenderer == NULL)
    {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    //Initialize renderer color
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    //Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }

    gScreenSurface = SDL_GetWindowSurface(gWindow);
    return success;
}

SDL_Texture *loadTexture(std::string path)
{
    SDL_Texture *newTexture = NULL;

    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), IMG_GetError());
        return NULL;
    }

    //Create texture from surface pixels
    newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
    if (newTexture == NULL)
    {
        printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
    }

    //Get rid of old loaded surface
    SDL_FreeSurface(loadedSurface);
    return newTexture;
}

bool loadMedia()
{
    //Loading success flag
    bool success = true;

    gTexture = loadTexture("assets/background.png");

    // //Load Foo' texture
    // if (!gFooTexture.loadFromFile("assets/foo.png"))
    // {
    //     printf("Failed to load Foo' texture image!\n");
    //     success = false;
    // }

    // //Load background texture
    // if (!gBackgroundTexture.loadFromFile("assets/background.png"))
    // {
    //     printf("Failed to load background texture image!\n");
    //     success = false;
    // }

    return success;
}

void close()
{
    //Free loaded images
    //delete &gFooTexture;
    //delete &gBackgroundTexture;
    SDL_DestroyTexture(gTexture);
    gTexture = NULL;

    //Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char *args[])
{

    if (!init())
    {
        printf("Failed to initialize!\n");
        return 0;
    }

    if (!loadMedia())
    {
        printf("Failed to load media!\n");
        return 0;
    }

    bool quit = false;
    SDL_Event event;

    while (!quit)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }
            //User presses a key
            else if (event.type == SDL_KEYDOWN)
            {
                //Select surfaces based on key press
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                }
            }
        }

        //Clear screen
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);

        SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

        //Render background texture to screen
        //gBackgroundTexture.render(0, 0);

        //Render Foo' to the screen
        //gFooTexture.render(240, 190);

        //Update screen
        SDL_RenderPresent(gRenderer);
    }

    close();

    return 0;
}