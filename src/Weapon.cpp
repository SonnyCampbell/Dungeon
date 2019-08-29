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

void UpdateWeapon(Weapon &weapon, double elapsed_game_time)
{
    if (weapon.isAttacking)
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

void DrawWeapon(Weapon &weapon, Vec2 position, int frame, bool facingRight)
{
    if (!weapon.isAttacking)
    {
        if (weapon.current_frame != frame)
        {
            weapon.direction += (frame == 1 || frame == 2) ? 1 : -1;
        }

        if (facingRight)
        {
            weapon.texture.renderF(position.x(), position.y() - weapon.offset.y() + weapon.direction, &weapon.spritesheet_clip, 90.f, NULL, SDL_FLIP_NONE);
        }
        else
        {
            weapon.texture.renderF(position.x() - weapon.offset.x(), position.y() - weapon.offset.y() + weapon.direction + 1, &weapon.spritesheet_clip, -90.f, NULL, SDL_FLIP_NONE);
        }

        weapon.current_frame = frame;
    }
    else
    {
        auto attack = weapon.basic_attack_framse[weapon.current_attack_frame];
        if (facingRight)
        {
            weapon.texture.renderF(position.x() + attack.attack_offset.x(), position.y() - weapon.offset.y() + attack.attack_offset.y(), &weapon.spritesheet_clip, attack.angle, NULL, SDL_FLIP_NONE);
        }
        else
        {
            weapon.texture.renderF(position.x() - weapon.offset.x() - attack.attack_offset.x(), position.y() - weapon.offset.y() + attack.attack_offset.y() + 1, &weapon.spritesheet_clip, -attack.angle, NULL, SDL_FLIP_NONE);
        }
    }
}

Weapon *createSword(SDL_Renderer **renderer, Vec2 position, int current_frame)
{
    return NewWeapon(renderer, {322, 25, 11, 22}, position, Vec2(11.f, 7.f), current_frame);
}
} // namespace WeaponManager
