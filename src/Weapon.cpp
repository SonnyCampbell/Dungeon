#include "Weapon.h"

namespace WeaponManager
{

Weapon *NewWeapon(SDL_Renderer **renderer, SDL_Rect clip, Vec2 position, Vec2 offset, int current_frame_count, int damage)
{
    //Don't load this from file every time - copy existing spritesheet texture
    auto texture = new LTexture(renderer);
    if (!texture->loadFromFile("assets/DungeonTilesetV2.png"))
    {
        printf("fuck");
    }
    auto rb = RigidBody(0.f, clip.w, clip.h, position, 0.f, Vec2::zero());

    auto weapon = new Weapon{*texture, rb, offset, clip, current_frame_count};
    weapon->damage = damage;
    weapon->targets_hit = std::set<int>();
    return weapon;
}

void DeleteWeapon()
{
}

void ResetFrames(Weapon &weapon)
{
    weapon.current_frame = 0;
    weapon.direction = 0;
}

void WeaponAttack(Weapon &weapon, AttackTypes new_attack_type)
{
    if (!weapon.isAttacking)
    {
        weapon.isAttacking = true;
        weapon.attack_type = new_attack_type;
        weapon.last_frame_time = Game::current_tick;
    }
}

void UpdateWeapon(Weapon &weapon, Vec2 center)
{

    weapon.rb.aabb = AABB{center, Vec2(weapon.spritesheet_clip.w / 2.f, weapon.spritesheet_clip.w / 2.f)};
    if (weapon.isAttacking && Game::current_tick - weapon.last_frame_time > weapon.frame_length)
    {
        weapon.last_frame_time = Game::current_tick;

        if (weapon.current_attack_frame >= 4)
        {
            weapon.isAttacking = false;
            weapon.current_attack_frame = 0;
            weapon.targets_hit.clear();
            ResetFrames(weapon);
            return;
        }
        weapon.current_attack_frame++;
    }
}

Vec2 GetWeaponPosition(Weapon &weapon, Vec2 camera_position, int frame, bool facingRight)
{
    auto position = weapon.rb.aabb.center - camera_position;
    if (!weapon.isAttacking)
    {
        if (weapon.current_frame != frame)
        {
            weapon.direction += (frame == 1 || frame == 2) ? 1 : -1;
        }

        if (facingRight)
        {
            return Vec2(position.x(), position.y() - weapon.offset.y() + weapon.direction);
        }
        else
        {
            return Vec2(position.x() - weapon.offset.x(), position.y() - weapon.offset.y() + weapon.direction + 1);
        }
    }
    else
    {
        auto attack = weapon.basic_attack_framse[weapon.current_attack_frame];
        if (facingRight)
        {
            return Vec2(position.x() + attack.attack_offset.x(), position.y() - weapon.offset.y() + attack.attack_offset.y());
        }
        else
        {
            return Vec2(position.x() - weapon.offset.x() - attack.attack_offset.x(), position.y() - weapon.offset.y() + attack.attack_offset.y() + 1);
        }
    }
}

void DrawWeapon(Weapon &weapon, Vec2 camera_position, int frame, bool facingRight)
{
    auto position = GetWeaponPosition(weapon, camera_position, frame, facingRight);
    if (!weapon.isAttacking)
    {

        if (facingRight)
        {
            weapon.texture.renderF(position.x(), position.y(), &weapon.spritesheet_clip, 90.f, NULL, SDL_FLIP_NONE);

            Vec2 collision_offset = {7, 8};
            weapon.collision_box = {(int)(position.x() + collision_offset.x()), (int)(position.y() + collision_offset.y()), 11, 11};
            SDL_SetRenderDrawColor(*weapon.texture.gRenderer, 0x00, 0x00, 0xFF, 0xFF);
            SDL_RenderDrawRect(*weapon.texture.gRenderer, &weapon.collision_box);
            SDL_SetRenderDrawColor(*weapon.texture.gRenderer, 0x00, 0xFF, 0x00, 0xFF);
            SDL_RenderDrawPointF(*weapon.texture.gRenderer, position.x(), position.y());
        }
        else
        {
            weapon.texture.renderF(position.x(), position.y(), &weapon.spritesheet_clip, -90.f, NULL, SDL_FLIP_NONE);

            Vec2 collision_offset = {7, 8};
            weapon.collision_box = {(int)(position.x() - weapon.offset.x() + collision_offset.x()), (int)(position.y() + collision_offset.y()), 11, 11};
            SDL_SetRenderDrawColor(*weapon.texture.gRenderer, 0x00, 0x00, 0xFF, 0xFF);
            SDL_RenderDrawRect(*weapon.texture.gRenderer, &weapon.collision_box);
            SDL_SetRenderDrawColor(*weapon.texture.gRenderer, 0x00, 0xFF, 0x00, 0xFF);
            SDL_RenderDrawPointF(*weapon.texture.gRenderer, position.x(), position.y());
        }

        weapon.current_frame = frame;
    }
    else
    {
        auto attack = weapon.basic_attack_framse[weapon.current_attack_frame];
        if (facingRight)
        {
            weapon.texture.renderF(position.x(), position.y(), &weapon.spritesheet_clip, attack.angle, NULL, SDL_FLIP_NONE);
            weapon.collision_box = {(int)(position.x() + attack.attack_collision_offset.x()), (int)(position.y() + attack.attack_collision_offset.y()), 11, 11};
            SDL_SetRenderDrawColor(*weapon.texture.gRenderer, 0x00, 0x00, 0xFF, 0xFF);
            SDL_RenderDrawRect(*weapon.texture.gRenderer, &weapon.collision_box);
            SDL_SetRenderDrawColor(*weapon.texture.gRenderer, 0x00, 0xFF, 0x00, 0xFF);
            SDL_RenderDrawPointF(*weapon.texture.gRenderer, position.x(), position.y());
        }
        else
        {
            weapon.texture.renderF(position.x(), position.y(), &weapon.spritesheet_clip, -attack.angle, NULL, SDL_FLIP_NONE);
            weapon.collision_box = {(int)(position.x() + attack.attack_collision_offset.x()), (int)(position.y() + attack.attack_collision_offset.y()), 11, 11};
            SDL_SetRenderDrawColor(*weapon.texture.gRenderer, 0x00, 0x00, 0xFF, 0xFF);
            SDL_RenderDrawRect(*weapon.texture.gRenderer, &weapon.collision_box);
            SDL_SetRenderDrawColor(*weapon.texture.gRenderer, 0x00, 0xFF, 0x00, 0xFF);
            SDL_RenderDrawPointF(*weapon.texture.gRenderer, position.x(), position.y());
        }
    }

    // #define PI 3.14159265
    //     auto pivot_point = weapon.rb.aabb.center;
    //     auto tranlate = position - pivot_point;
    //     auto x_new = cos(90 * PI / 180) * tranlate.x() - sin(90 * PI / 180) * tranlate.y();
    //     auto y_new = sin(90 * PI / 180) * tranlate.x() + cos(90 * PI / 180) * tranlate.y();
    //     Vec2 new_point = {(float)x_new + pivot_point.x(), (float)y_new + pivot_point.y()};

    //     // DEBUG DRAWING
    //     //DUMB - Rotate around point, not around origin

    //     auto x = cos(90 * PI / 180) * position.x() - sin(90 * PI / 180) * position.y();
    //     auto y = sin(90 * PI / 180) * position.x() + cos(90 * PI / 180) * position.y();
    //     SDL_FRect debug_rect = {new_point.x(), new_point.y(), (float)weapon.spritesheet_clip.h, (float)weapon.spritesheet_clip.w};
    //     SDL_SetRenderDrawColor(*weapon.texture.gRenderer, 0x00, 0x00, 0xFF, 0xFF);
    //     SDL_RenderDrawRectF(*weapon.texture.gRenderer, &debug_rect);
    //     SDL_SetRenderDrawColor(*weapon.texture.gRenderer, 0xFF, 0x00, 0x00, 0xFF);
}

Weapon *createSword(SDL_Renderer **renderer, Vec2 position, int current_frame)
{
    return NewWeapon(renderer, {322, 25, 11, 22}, position, Vec2(11.f, 7.f), current_frame, 20);
}

Weapon *createNailBoard(SDL_Renderer **renderer, Vec2 position, int current_frame)
{
    return NewWeapon(renderer, {322, 56, 11, 22}, position, Vec2(11.f, 7.f), current_frame, 15);
}

Weapon *createHeavyMace(SDL_Renderer **renderer, Vec2 position, int current_frame)
{
    return NewWeapon(renderer, {338, 54, 11, 26}, position, Vec2(11.f, 7.f), current_frame, 30);
}
} // namespace WeaponManager
