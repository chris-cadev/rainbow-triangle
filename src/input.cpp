#include "input.h"

InputState GetInput()
{
    InputState inputState = {};

    inputState.left   = IsKeyPressed(KEY_LEFT)  || IsKeyPressedRepeat(KEY_LEFT)  ||
                        IsKeyPressed(KEY_A)     || IsKeyPressedRepeat(KEY_A)     ||
                        IsKeyPressed(KEY_H)     || IsKeyPressedRepeat(KEY_H);
    inputState.right  = IsKeyPressed(KEY_RIGHT) || IsKeyPressedRepeat(KEY_RIGHT) ||
                        IsKeyPressed(KEY_D)     || IsKeyPressedRepeat(KEY_D)     ||
                        IsKeyPressed(KEY_L)     || IsKeyPressedRepeat(KEY_L);
    inputState.up    = IsKeyPressed(KEY_UP)    || IsKeyPressedRepeat(KEY_UP)    ||
                        IsKeyPressed(KEY_W)    || IsKeyPressedRepeat(KEY_W)     ||
                        IsKeyPressed(KEY_K)    || IsKeyPressedRepeat(KEY_K);
    inputState.down  = IsKeyPressed(KEY_DOWN)  || IsKeyPressedRepeat(KEY_DOWN)  ||
                        IsKeyPressed(KEY_S)    || IsKeyPressedRepeat(KEY_S)     ||
                        IsKeyPressed(KEY_J)    || IsKeyPressedRepeat(KEY_J);
    inputState.action = IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER);

    Vector2 pos = GetMousePosition();
    int touchCount = GetTouchPointCount();
    if (touchCount > 0)
        pos = GetTouchPosition(0);
    inputState.pointerPosition = pos;
    inputState.pointerClicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    inputState.pointerRightClicked = IsMouseButtonPressed(MOUSE_BUTTON_RIGHT);

    if (touchCount > 0 && inputState.pointerClicked)
    {
        if (pos.x < GetScreenWidth() / 2.0f)
            inputState.left = true;
        else
            inputState.right = true;
    }

    return inputState;
}
