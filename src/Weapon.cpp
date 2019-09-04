#include "Weapon.h"

namespace WeaponManager
{

Weapon *NewWeapon(SDL_Renderer **renderer, SDL_Rect clip, Vec2 position, Vec2 offset, int current_frame_count)
{
    //Don't load this from file every time - copy existing spritesheet texture
    auto texture = new LTexture(renderer);
    if (!texture->loadFromFile("assets/DungeonTilesetV2.png"))
    {
        printf("fuck");
    }
    auto rb = RigidBody(0.f, clip.w, clip.h, position, 0.f, Vec2::zero());

    return new Weapon{*texture, rb, offset, clip, current_frame_count};
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
    printf("Attack!\n");
    if (!weapon.isAttacking)
    {
        weapon.isAttacking = true;
        weapon.attack_type = new_attack_type;
    }
}

void UpdateWeapon(Weapon &weapon, Vec2 center, double elapsed_game_time)
{
    if (!weapon.isAttacking)
    {
        weapon.rb.aabb = AABB{center, Vec2(weapon.spritesheet_clip.w / 2.f, weapon.spritesheet_clip.w / 2.f)};
        //weapon.collision_box =
    }
    else
    {
        if (elapsed_game_time - weapon.last_frame_time > weapon.frame_length)
        {

            weapon.last_frame_time = elapsed_game_time;
            auto next_frame = weapon.current_attack_frame + 1;
            if (next_frame >= 4)
            {
                weapon.isAttacking = false;
                weapon.current_attack_frame = 0;
                ResetFrames(weapon);
                return;
            }
            weapon.current_attack_frame = (next_frame) % 4;
        }
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
        }
        else
        {
            weapon.texture.renderF(position.x(), position.y(), &weapon.spritesheet_clip, -90.f, NULL, SDL_FLIP_NONE);
        }

        weapon.current_frame = frame;
    }
    else
    {
        auto attack = weapon.basic_attack_framse[weapon.current_attack_frame];
        if (facingRight)
        {
            weapon.texture.renderF(position.x(), position.y(), &weapon.spritesheet_clip, attack.angle, NULL, SDL_FLIP_NONE);
        }
        else
        {
            weapon.texture.renderF(position.x(), position.y(), &weapon.spritesheet_clip, -attack.angle, NULL, SDL_FLIP_NONE);
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
    return NewWeapon(renderer, {322, 25, 11, 22}, position, Vec2(11.f, 7.f), current_frame);
}

Weapon *createNailBoard(SDL_Renderer **renderer, Vec2 position, int current_frame)
{
    return NewWeapon(renderer, {322, 56, 11, 22}, position, Vec2(11.f, 7.f), current_frame);
}

Weapon *createHeavyMace(SDL_Renderer **renderer, Vec2 position, int current_frame)
{
    return NewWeapon(renderer, {338, 54, 11, 26}, position, Vec2(11.f, 7.f), current_frame);
}
} // namespace WeaponManager
