#include "raylib.h"

int main()
{
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "Rainbow Triangle - Lab Setup");
    MaximizeWindow();
    const int screenWidth  = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    Image image = GenImageColor(screenWidth, screenHeight, BLANK);
    Color *pixels = (Color *)image.data;
    for (int x = 0; x < screenWidth; x++)
    {
        Color color = ColorFromHSV((float)x / screenWidth * 360.0f, 1.0f, 1.0f);
        for (int y = 0; y < screenHeight; y++)
            pixels[y * screenWidth + x] = color;
    }
    Texture2D rainbow = LoadTextureFromImage(image);
    UnloadImage(image);

    float offset = 0.0f;

    while (!WindowShouldClose())
    {
        offset -= 2.0f;
        if (offset < 0) offset += screenWidth;

        BeginDrawing();
        DrawTexture(rainbow, (int)offset, 0, WHITE);
        DrawTexture(rainbow, (int)offset - screenWidth, 0, WHITE);
        EndDrawing();
    }

    UnloadTexture(rainbow);

    CloseWindow();
    return 0;
}
