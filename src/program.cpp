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
#include "Enemies/Enemy.h"
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
//auto enemy_states = FSMTableState::StateMachineData();
//auto enemy_sprites = std::vector<AnimatedSprite>();

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

void render(SDL_Renderer *renderer, LTexture &texture, TMXLoader *loader, QuadTree *quad)
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

    for (auto enemy : enemies)
    {
        enemy.draw();
    }

    quad->draw(gRenderer); // Debug Drawing

    for (auto debug_rect : Game::debug_rects)
    {
        SDL_SetRenderDrawColor(gRenderer, debug_rect.colour.r, debug_rect.colour.g, debug_rect.colour.b, debug_rect.colour.a);
        SDL_RenderDrawRectF(gRenderer, &debug_rect.rect);
        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
    }
    Game::debug_rects.clear();
}

void UpdateEnemies()
{
    //enemy_states.UpdateStates(Game::tick_delta(), player);

    // for (int i = 0; i < enemy_sprites.size(); i++)
    // {
    //     UpdateEnemy(enemy_sprites[i], player);
    // }

    std::vector<Enemy *> pendingChase = std::vector<Enemy *>();
    std::vector<Enemy *> pendingIdle = std::vector<Enemy *>();
    std::vector<Enemy *> pendingReadyAttack = std::vector<Enemy *>();
    std::vector<Enemy *> pendingAttack = std::vector<Enemy *>();

    for (auto enemy = enemies.begin(); enemy != enemies.end();)
    {
        auto separation_vector = player.rb.aabb.center - enemy->rb.aabb.center;
        auto dist = separation_vector.length();
        switch (enemy->state)
        {
        case EnemyState1::idle:
            if (dist < 100 && dist > 8 && (Game::current_tick - enemy->attack.most_recent_attack_time > 1000))
            {
                //printf("%i taking chase..\n", enemy->id);
                enemy->rb.direction = separation_vector.normalized_vector();

                // enemy->rb.aabb.center = enemy->rb.aabb.center + (enemy->rb.direction * enemy->rb.speed * Game::tick_delta());
                //pendingChase.push_back(&*enemy);
            }

            ++enemy;

            break;

        case EnemyState1::chasing:
            if (dist >= 100)
            {
                printf("%i lost sight..\n", enemy->id);
                enemy->rb.direction = Vec2::zero();
                pendingIdle.push_back(&*enemy);
                ++enemy;
            }
            else if (dist < 20)
            {
                printf("%i attacking player \n", enemy->id);
                enemy->rb.direction = Vec2::zero();
                enemy->beginAttack(player);
                pendingReadyAttack.push_back(&*enemy);
                ++enemy;
            }
            else
            {
                enemy->rb.direction = separation_vector.normalized_vector();
                enemy->rb.aabb.center = enemy->rb.aabb.center + (enemy->rb.direction * enemy->rb.speed * Game::tick_delta());
                ++enemy;
            }
            break;
        case EnemyState1::readying_attack:
            enemy->continueAttack(player);
            if (!enemy->attack.readying_attack)
            {
                pendingAttack.push_back(&*enemy);
            }
            ++enemy;
            break;
        case EnemyState1::attacking:
            enemy->finishAttack(player);
            pendingIdle.push_back(&*enemy);
            enemy->attack.most_recent_attack_time = Game::current_tick;
            ++enemy;
            break;
        }
    }

    for (auto enemy : pendingChase)
    {
        enemy->state = EnemyState1::chasing;
        //enemy.update();
    }

    for (auto enemy : pendingIdle)
    {
        enemy->state = EnemyState1::idle;
    }

    for (auto enemy : pendingReadyAttack)
    {
        enemy->state = EnemyState1::readying_attack;
    }

    for (auto enemy : pendingAttack)
    {
        enemy->state = EnemyState1::attacking;
    }

    for (auto enemy : enemies)
    {
        enemy.update();
    }
}

bool CheckEnemyCollisions(QuadTree *quad)
{
    bool enemyCollision = false;
    std::vector<int> killed_enemies = std::vector<int>();
    std::vector<QuadCollionObject> possible_collisions = std::vector<QuadCollionObject>();
    quad->retrieve(possible_collisions, {player.id, player.rb.aabb.boundingBox()});
    for (auto &collision : possible_collisions)
    {
        if (collision.id == player.id)
        {
            continue;
        }

        auto it1 = std::find_if(enemies.begin(), enemies.end(),
                                [collision](const Enemy val) {
                                    return val.id == collision.id;
                                });

        if (it1 == enemies.end())
            continue;

        auto test = Collision::entityCollide(player.rb, it1->rb);
        if (test.collision)
        {
            auto correction = Collision::getCollisionCorrectedDirection(player.rb, test);
            player.rb.aabb.center = player.rb.aabb.center + (correction * player.rb.speed * Game::tick_delta());
            printf("bump \n");
            enemyCollision = true;
        }

        if (player.weapon->isAttacking && player.weapon->targets_hit.find(collision.id) == player.weapon->targets_hit.end())
        {
            auto hit = Collision::checkBoxCollision(player.weapon->collision_box, collision.collision_rect);
            if (hit)
            {
                auto it = std::find_if(enemies.begin(), enemies.end(),
                                       [collision](const Enemy val) {
                                           return val.id == collision.id;
                                       });

                if (it == enemies.end())
                    continue;

                Enemy &hitEnemy = *it;
                hitEnemy.takeDamage(player.weapon->damage);
                hitEnemy.takeHit(15, player.rb.aabb.center);

                player.weapon->targets_hit.insert(collision.id);

                if (hitEnemy.stats.health <= 0)
                {
                    //enemies.erase(it);
                    //killed_enemies.push_back(hitEnemy.id);
                }
            }
        }
    }

    for (auto id : killed_enemies)
    {
        EnemyManager::DeleteEnemyById(enemies, id);
    }

    return enemyCollision;
}

void Update(QuadTree *quad)
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

        //Need to handle entity collision in here too!

        Game::UpdateCamera(player.rb.aabb.center);

        UpdateEnemies();

        CheckEnemyCollisions(quad);

        return;
    }

    Game::UpdateCamera(player.rb.aabb.center);
    UpdateEnemies();

    auto enemyCollision = CheckEnemyCollisions(quad);
    if (!enemyCollision)
    {
        Vec2 currentDirection = Vec2(player.rb.direction.x(), player.rb.direction.y());
        currentDirection.normalize();
        player.rb.aabb.center = player.rb.aabb.center + (currentDirection * player.rb.speed * Game::tick_delta());
    }
}

void BuildQuadTree(QuadTree *quad, std::vector<Enemy> enemies)
{
    for (auto enemy : enemies)
    {
        quad->insert({enemy.id, enemy.rb.aabb.boundingBox()});
    }

    quad->insert({player.id, player.rb.aabb.boundingBox()});
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
    enemies.push_back(NewEnemy1(&gRenderer, {100, 180}));
    // enemies.push_back(NewEnemy1(&gRenderer, {200, 180}));
    // enemies.push_back(NewEnemy1(&gRenderer, {100, 280}));
    // enemies.push_back(NewEnemy1(&gRenderer, {200, 280}));

    QuadTree *quad = new QuadTree(0, {0, 0, 600, 600});

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
        BuildQuadTree(quad, enemies);
        Update(quad);

        //CheckEnemyCollisions(quad);

        render(gRenderer, gSpriteSheetTexture, loader, quad);

        //Update screen
        SDL_RenderPresent(gRenderer);

        float seconds = (Game::current_tick / 1000.f);
        float fps = (float)totalFrames++ / seconds;
    }

    close();

    return 0;
}
