
#pragma once

#include "../../Vec2.h"

class E1BasicAttack
{
public:
    Vec2 target_position;
    int last_frame_time;

    Vec2 attack_prep[4] = {Vec2(5.f, 0.f), Vec2(-5.f, 0.f), Vec2(5.f, 0.f), Vec2(-5.f, 0.f)};
    float frame_length = 120.f;
    float attack_speed = 60.f;

    bool readying_attack = false;
    bool finished_attack = false;
    int ready_frame = 0;
    long most_recent_attack_time = 0;
};