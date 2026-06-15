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
    inputState.action = IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER);

    Vector2 pos = GetMousePosition();
    int touchCount = GetTouchPointCount();
    if (touchCount > 0)
        pos = GetTouchPosition(0);
    inputState.pointerPos = pos;
    inputState.pointerClicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    inputState.pointerRightClicked = IsMouseButtonPressed(MOUSE_BUTTON_RIGHT);

    return inputState;
}
