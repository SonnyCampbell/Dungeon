#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <unordered_map>

#include "Game Components/Game.h"
#include "LTexture.h"
#include "AnimatedSprite.h"
#include "AnimationKey.h"
#include "Player.h"
#include "EnemyManager.h"
#include "Structs/Enemy.h"
#include "TMXLoader/TMXLoader.h"
#include "Collision.h"
#include "QuadTree.h"
#include "State/FSMTableState.h"

using namespace PlayerManager;
using namespace EnemyManager;

// Global Window
SDL_Window *gWindow = NULL;

//The window renderer
SDL_Renderer *gRenderer = NULL;

auto loader = new TMXLoader();
TMXMap *map1 = NULL;

LTexture gSpriteSheetTexture(&gRenderer);

//The surface contained by the window
SDL_Surface *gScreenSurface = NULL;

Player player = {};
std::vector<Enemy> enemies = std::vector<Enemy>();
auto enemy_states = FSMTableState::StateMachineData();

bool init()
{
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    //The window we'll be rendering to
    gWindow = SDL_CreateWindow("SDL2 Pixel Drawing", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Game::screen_width, Game::screen_height, 0);
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
    SDL_RenderSetScale(gRenderer, Game::render_scale, Game::render_scale);

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

    loader->loadMap("Map1", "assets/Map1.tmx");
    map1 = loader->getMap("Map1");

    //Load sprite sheet texture
    if (!gSpriteSheetTexture.loadFromFile("assets/DungeonTilesetV2.png"))
    {
        printf("Failed to load sprite sheet texture!\n");
        success = false;
    }

    return success;
}

void close()
{
    //Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

void render(SDL_Renderer *renderer, LTexture &texture, TMXLoader *loader)
{
    unsigned int tileID = 0;

    int tileWidth = map1->getTileWidth();
    int tileHeight = map1->getTileHeight();

    auto layers = map1->getTileLayers();

    for (unsigned int index = 0; index < layers->size(); ++index) // layers->size() = 5
    {
        auto layer = layers->at(index);
        for (int i = 0; i < map1->getWidth(); ++i)
        {
            for (int j = 0; j < map1->getHeight(); ++j)
            {
                tileID = layer.getTileVector()[i][j]->getTileID();

                // only render if it is an actual tile (tileID = 0 means no tile / don't render anycollision_corrected_direction here)
                if (tileID > 0)
                {
                    SDL_Rect srcrect = {(int)((tileID - 1) % 32) * tileWidth, (int)((tileID - 1) / 32) * tileHeight, tileWidth, tileHeight}; // TODO Constant 32 = tiles wide/high (2 extra = layer around map?)
                    auto destX = i * tileWidth - Game::camera.x;
                    auto destY = j * tileHeight - Game::camera.y;
                    texture.render(destX, destY, &srcrect);

                    //DEBUG DRAWING
                    // if (index == 2)
                    // {
                    //     SDL_FRect rect = {destX, destY, tileWidth, tileHeight};
                    //     SDL_Colour colour = {0x00, 0x00, 0xFF, 0xFF};
                    //     Game::debug_rects.push_back({rect, colour});
                    // }
                }
            }
        }
    }

    DrawPlayer(player);
}

void Update()
{
    UpdatePlayer(player);
    auto collision_layer = map1->getTileLayers()->at(2);
    auto collisions = Collision::collision(collision_layer, *map1->getTileSet("16bit Dungeon Tiles II"), player.rb, Game::tick_delta(), gRenderer);

    if (collisions.size() > 0)
    {
        auto collision_vector = Vec2(0, 0);

        for (int i = 0; i < collisions.size(); i++)
        {
            auto collision_normal = collisions[i].contact.normal;
            if ((collision_normal.x() > 0.f && player.rb.direction.x() < 0.f) || (collision_normal.x() < 0.f && player.rb.direction.x() > 0.f))
            {
                collision_vector = Vec2(1.f, collision_vector.y());
                //player.rb.direction = Vec2(0.f, player.rb.direction.y());
            }
            if ((collision_normal.y() > 0.f && player.rb.direction.y() < 0.f) || (collision_normal.y() < 0.f && player.rb.direction.y() > 0.f))
            {
                collision_vector = Vec2(collision_vector.x(), 1.f);
                //player.rb.direction = Vec2(player.rb.direction.x(), 0.f);
            }
        }

        Vec2 currentDirection = Vec2(player.rb.direction.x(), player.rb.direction.y());
        currentDirection.normalize();
        auto collision_corrected_direction =
            Vec2(currentDirection.x() - (currentDirection.x() * collision_vector.x()),
                 currentDirection.y() - (currentDirection.y() * collision_vector.y()));

        player.rb.aabb.center = player.rb.aabb.center + (collision_corrected_direction * player.rb.speed * Game::tick_delta());

        Game::UpdateCamera(player.rb.aabb.center);

        return;
    }

    Vec2 currentDirection = Vec2(player.rb.direction.x(), player.rb.direction.y());
    currentDirection.normalize();
    player.rb.aabb.center = player.rb.aabb.center + (currentDirection * player.rb.speed * Game::tick_delta());

    Game::UpdateCamera(player.rb.aabb.center);
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

        PlayerHandleInputEvent(event, player);
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

    player = NewPlayer(&gRenderer, {100, 100});
    auto enemy1 = NewEnemy1(&gRenderer, {100, 180});
    //enemies.push_back(enemy1);
    enemy_states.idles.push_back(enemy1);

    QuadTree *quad = new QuadTree(0, {0, 0, 480, 480});

    int totalFrames = 0;
    bool quit = false;
    SDL_Event event;
    while (!quit)
    {
        Game::last_tick = Game::current_tick;
        Game::current_tick = SDL_GetTicks();

        //Clear screen
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);
        quad->clear();

        HandleInput(event, quit);
        Update();
        enemy_states.UpdateStates(Game::tick_delta(), player);
        enemies = enemy_states.AllEnemies();
        for (auto &enemy : enemies)
        {
            UpdateEnemy(enemy, player);
        }

        render(gRenderer, gSpriteSheetTexture, loader);
        for (auto &enemy : enemies)
        {
            DrawEnemy(enemy);
            quad->insert({enemy.id, enemy.rb.aabb.boundingBox()});
        }

        quad->insert({player.id, player.rb.aabb.boundingBox()});
        quad->draw(gRenderer); // Debug Drawing

        std::vector<QuadCollionObject> possible_collisions = std::vector<QuadCollionObject>();
        quad->retrieve(possible_collisions, {player.id, player.rb.aabb.boundingBox()});
        for (auto &collision : possible_collisions)
        {
            if (collision.id == player.id)
                continue;

            if (player.weapon->isAttacking && player.weapon->targets_hit.find(collision.id) == player.weapon->targets_hit.end())
            {
                auto hit = Collision::checkBoxCollision(player.weapon->collision_box, collision.collision_rect);
                if (hit)
                {
                    auto it = std::find_if(enemies.begin(), enemies.end(),
                                           [collision](const Enemy val) {
                                               if (val.id == collision.id)
                                                   return true;
                                               return false;
                                           });

                    if (it == enemies.end())
                    {
                        continue;
                    }

                    auto hitEnemy = *it;
                    EnemyManager::TakeDamage(hitEnemy, player.weapon->damage);
                    player.weapon->targets_hit.insert(collision.id);
                    printf("Hit \n");

                    // if (hitEnemy->stats.health <= 0)
                    // {
                    //     enemies.erase(it);
                    //     EnemyManager::DeleteEnemy(**it);
                    // }
                }
            }
        }

        for (auto debug_rect : Game::debug_rects)
        {
            SDL_SetRenderDrawColor(gRenderer, debug_rect.colour.r, debug_rect.colour.g, debug_rect.colour.b, debug_rect.colour.a);
            SDL_RenderDrawRectF(gRenderer, &debug_rect.rect);
            SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
        }
        Game::debug_rects.clear();

        //Update screen
        SDL_RenderPresent(gRenderer);

        float seconds = (Game::current_tick / 1000.f);
        float fps = (float)totalFrames++ / seconds;
    }

    close();

    return 0;
}
