#pragma once
#include "AnimatedSprite.h"
#include "Vec2.h"

class Player
{
private:
public:
    AnimatedSprite *sprite;
    Vec2 position;
    float speed;
    Vec2 *direction;

    Player(SDL_Renderer **renderer, Vec2 _position, float _speed = 166.0f)
    {
        position = _position;
        speed = _speed;
        direction = new Vec2(0, 0);

        Vec2 frameSize = {16, 28};
        int runFps = 10;
        int idleFps = 5;
        std::map<AnimationKey, Animation *> *animations = new std::map<AnimationKey, Animation *>({{IdleUp, new Animation(4, idleFps, frameSize, {128, 36})},
                                                                                                   {IdleDown, new Animation(4, idleFps, frameSize, {128, 36})},
                                                                                                   {IdleLeft, new Animation(4, idleFps, frameSize, {128, 36})},
                                                                                                   {IdleRight, new Animation(4, idleFps, frameSize, {128, 36})},
                                                                                                   {WalkUp, new Animation(4, runFps, frameSize, {192, 36})},
                                                                                                   {WalkDown, new Animation(4, runFps, frameSize, {192, 36})},
                                                                                                   {WalkLeft, new Animation(4, runFps, frameSize, {192, 36})},
                                                                                                   {WalkRight, new Animation(4, runFps, frameSize, {192, 36})}});

        sprite = new AnimatedSprite(renderer, "assets/DungeonTilesetV2.png", animations, IdleUp);
    }

    ~Player()
    {
        delete sprite;
    }

    void Draw();
    void Update(double currentTick);
    void MovePlayer(Vec2 movementVector, float dt);
    void HandleInputEvent(const SDL_Event &event, float dt);
    void DoThing(Vec2 movementVector);
};

void Player::Update(double currentTick)
{
    Vec2 currentDirection = Vec2(direction->x(), direction->y());
    currentDirection.normalize();
    position = position + (currentDirection * 2.5);

    sprite->UpdateAnimation(sprite->currentAnimationKey, currentTick);
}

void Player::Draw()
{
    sprite->Draw(position);
}

void Player::HandleInputEvent(const SDL_Event &event, float dt)
{
    printf("HandleEvent \n");
    // const Uint8 *keys = SDL_GetKeyboardState(NULL);

    // if (keys[SDL_SCANCODE_W])
    // {
    //     MovePlayer(Vec2(0, -1), dt);
    // }
    // else if (keys[SDL_SCANCODE_S])
    // {
    //     MovePlayer(Vec2(0, 1), dt);
    // }
    // else if (keys[SDL_SCANCODE_A])
    // {
    //     MovePlayer(Vec2(-1, 0), dt);
    // }
    // else if (keys[SDL_SCANCODE_D])
    // {
    //     printf("Move Right \n");
    //     MovePlayer(Vec2(1, 0), dt);
    // }

    if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
    {
        //Select surfaces based on key press
        switch (event.key.keysym.sym)
        {
        // case SDLK_ESCAPE:
        //     quit = true;
        //     break;
        case SDLK_w:
            //flipType = SDL_FLIP_NONE;

            DoThing(Vec2(0, -1));
            break;
        case SDLK_s:
            //degrees += 60;
            DoThing(Vec2(0, 1));
            break;
        case SDLK_a:
            //degrees -= 60;
            DoThing(Vec2(-1, 0));
            break;
        case SDLK_d:
            //degrees += 60;
            printf("Move Right \n");
            DoThing(Vec2(1, 0));
            break;
        }
    }
    else if (event.type == SDL_KEYUP && event.key.repeat == 0)
    {
        //Select surfaces based on key press
        switch (event.key.keysym.sym)
        {
        // case SDLK_ESCAPE:
        //     quit = true;
        //     break;
        case SDLK_w:
            //flipType = SDL_FLIP_NONE;

            DoThing(Vec2(0, 1));
            break;
        case SDLK_s:
            //degrees += 60;
            DoThing(Vec2(0, -1));
            break;
        case SDLK_a:
            //degrees -= 60;
            DoThing(Vec2(1, 0));
            break;
        case SDLK_d:
            //degrees += 60;
            printf("Move Right \n");
            DoThing(Vec2(-1, 0));
            break;
        }
    }
}

void Player::MovePlayer(Vec2 movementVector, float dt)
{
    movementVector.normalize();
    Vec2 vel = movementVector * 2.5f;
    printf("Position: %f %f \n", position.x(), position.y());
    printf("Vel: %f %f \n", vel.x(), vel.y());
    position += vel;
}

void Player::DoThing(Vec2 movementVector)
{
    *direction += movementVector;
}
