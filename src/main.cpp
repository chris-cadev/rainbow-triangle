#include "raylib.h"
#include "colors.h"
#include "render.h"
#include "state.h"
#include "input.h"
#include "sounds.h"

int main()
{
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "Rainbow Triangle");
    InitAudioDevice();
    MaximizeWindow();
    SetTargetFPS(144);

    SoundBank sounds = LoadAllSounds();

    GameState state;
    InitGame(state, GetScreenWidth(), GetScreenHeight());

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        if (dt > 0.05f) dt = 0.05f;
        int sw = GetScreenWidth();
        int sh = GetScreenHeight();

        InputState input = GetInput();
        UpdateGame(state, input, sw, sh, dt, sounds);

        BeginDrawing();
        DrawScene(state, sw, sh);
        EndDrawing();
    }

    UnloadAllSounds(sounds);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
