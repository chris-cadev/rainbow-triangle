#pragma once
#include "raylib.h"

struct InputState
{
    bool left;
    bool right;
    bool action;
    Vector2 pointerPos;
    bool pointerClicked;
    bool pointerRightClicked;
};

InputState GetInput();
