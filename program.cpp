#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

#include "LTexture.h"
#include "src/AnimatedSprite.h"
#include "src/AnimationKey.h"
#include "src/Player.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

double lastTick = 0;
double currentTick = 0;
float dt = 0.0f;

// Global Window
SDL_Window *gWindow = NULL;

//The window renderer
SDL_Renderer *gRenderer = NULL;

//Scene textures
LTexture gFooTexture(&gRenderer);
LTexture gBackgroundTexture(&gRenderer);

//Walking animation
const int WALKING_ANIMATION_FRAMES = 4;
SDL_Rect gSpriteClips[WALKING_ANIMATION_FRAMES];
LTexture gSpriteSheetTexture(&gRenderer);

//The surface contained by the window
SDL_Surface *gScreenSurface = NULL;

Player *player = NULL;

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

    //Create vsynced renderer for window
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (gRenderer == NULL)
    {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        success = false;
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

    //Load Foo' texture
    if (!gFooTexture.loadFromFile("assets/foo.png"))
    {
        printf("Failed to load Foo' texture image!\n");
        success = false;
    }

    //Load background texture
    if (!gBackgroundTexture.loadFromFile("assets/background.png"))
    {
        printf("Failed to load background texture image!\n");
        success = false;
    }

    //Load sprite sheet texture
    if (!gSpriteSheetTexture.loadFromFile("assets/DungeonTilesetV2.png"))
    {
        printf("Failed to load sprite sheet texture!\n");
        success = false;
    }
    else
    {
        int width = 16;
        int height = 28;

        //Set top left sprite
        gSpriteClips[0].x = 192;
        gSpriteClips[0].y = 36;
        gSpriteClips[0].w = width;
        gSpriteClips[0].h = height;

        //Set top right sprite
        gSpriteClips[1].x = 208;
        gSpriteClips[1].y = 36;
        gSpriteClips[1].w = width;
        gSpriteClips[1].h = height;

        //Set bottom left sprite
        gSpriteClips[2].x = 224;
        gSpriteClips[2].y = 36;
        gSpriteClips[2].w = width;
        gSpriteClips[2].h = height;

        //Set bottom right sprite
        gSpriteClips[3].x = 240;
        gSpriteClips[3].y = 36;
        gSpriteClips[3].w = width;
        gSpriteClips[3].h = height;
    }

    return success;
}

void close()
{
    //Free loaded images
    gFooTexture.free();
    gBackgroundTexture.free();

    //Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

void HandleInput(SDL_Event &event, bool &quit)
{
    while (SDL_PollEvent(&event) != 0)
    {
        if (event.type == SDL_QUIT)
        {
            quit = true;
        }
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

        player->HandleInputEvent(event, dt);
    }
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

    //Current animation frame
    int frame = 0;

    //Flip type
    SDL_RendererFlip flipType = SDL_FLIP_NONE;

    player = new Player(&gRenderer, {50, 50});

    // TODO: Need gametime for animation frames
    while (!quit)
    {
        currentTick = SDL_GetTicks();
        dt = (currentTick - lastTick) / 1000.0f;
        lastTick = currentTick;

        HandleInput(event, quit);

        //Clear screen
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);

        //Render background texture to screen
        //gBackgroundTexture.render(0, 0);

        //Render Foo' to the screen
        //gFooTexture.render(240, 190);

        //Render current frame
        SDL_Rect *currentClip = &gSpriteClips[frame / 6];
        gSpriteSheetTexture.render((SCREEN_WIDTH - currentClip->w) / 2, (SCREEN_HEIGHT - currentClip->h) / 2, currentClip, NULL, NULL, flipType);

        //Go to next frame
        ++frame;

        //Cycle animation
        if (frame / 6 >= WALKING_ANIMATION_FRAMES)
        {
            frame = 0;
        }

        player->Update(currentTick, dt);
        player->Draw();

        //Update screen
        SDL_RenderPresent(gRenderer);
    }

    close();

    return 0;
}