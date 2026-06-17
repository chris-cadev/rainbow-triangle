#pragma once
#include "raylib.h"

struct InputState
{
    bool left;
    bool right;
    bool up;
    bool down;
    bool action;
    Vector2 pointerPosition;
    bool pointerClicked;
    bool pointerRightClicked;
};

InputState GetInput();
