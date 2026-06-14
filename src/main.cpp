#include "raylib.h"
#include "colors.h"
#include "render.h"
#include "state.h"
#include "input.h"
#include "sounds.h"

#define INITIAL_WIDTH 400

int main()
{
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(INITIAL_WIDTH, INITIAL_WIDTH * 2, "Rainbow Triangle");
    InitAudioDevice();
    SetTargetFPS(144);

    SoundBank sounds = LoadAllSounds();

    GameState state;
    InitGame(state);

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        if (dt > 0.05f) dt = 0.05f;
        int sw = GetScreenWidth();
        int sh = GetScreenHeight();

        // Enforce 1:2 aspect ratio, height-driven, clamped to monitor width
        int newSh = sh;
        int newSw = sh / 2;
        int maxW = GetMonitorWidth(GetCurrentMonitor());
        if (newSw > maxW)
        {
            newSw = maxW;
            newSh = maxW * 2;
        }
        if (newSw != sw || newSh != sh)
            SetWindowSize(newSw, newSh);
        sw = newSw;
        sh = newSh;

        int giw = sw;
        int gih = sh;

        InputState input = GetInput();
        UpdateGame(state, input, giw, gih, dt, sounds);

        BeginDrawing();
        ClearBackground(BLACK);
        DrawScene(state, giw, gih);
        EndDrawing();
    }

    UnloadAllSounds(sounds);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
