#include "input.h"

InputState GetInput()
{
    InputState in = {};

    in.left   = IsKeyPressed(KEY_LEFT)  || IsKeyPressedRepeat(KEY_LEFT)  ||
                IsKeyPressed(KEY_A)     || IsKeyPressedRepeat(KEY_A)     ||
                IsKeyPressed(KEY_H)     || IsKeyPressedRepeat(KEY_H);
    in.right  = IsKeyPressed(KEY_RIGHT) || IsKeyPressedRepeat(KEY_RIGHT) ||
                IsKeyPressed(KEY_D)     || IsKeyPressedRepeat(KEY_D)     ||
                IsKeyPressed(KEY_L)     || IsKeyPressedRepeat(KEY_L);
    in.action = IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER);

    return in;
}
