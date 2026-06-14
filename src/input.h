#pragma once
#include "raylib.h"

struct InputState
{
    bool left;
    bool right;
    bool action;
};

InputState GetInput();
