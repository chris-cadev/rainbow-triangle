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
    InitGame(state, 400, 800);

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        if (dt > 0.05f) dt = 0.05f;
        int sw = GetScreenWidth();
        int sh = GetScreenHeight();

        // 1:2 game area centered in window
        float gw = (float)sw;
        float gh = gw * 2.0f;
        if (gh > (float)sh)
        {
            gh = (float)sh;
            gw = gh / 2.0f;
        }
        int giw = (int)gw;
        int gih = (int)gh;
        float ox = ((float)sw - gw) / 2.0f;
        float oy = ((float)sh - gh) / 2.0f;

        InputState input = GetInput();
        UpdateGame(state, input, giw, gih, dt, sounds);

        Camera2D cam = { 0 };
        cam.offset = (Vector2){ ox, oy };
        cam.zoom = 1.0f;

        BeginDrawing();
        ClearBackground(BLACK);
        BeginScissorMode((int)ox, (int)oy, giw, gih);
        BeginMode2D(cam);
        DrawScene(state, giw, gih);
        EndMode2D();
        EndScissorMode();
        EndDrawing();
    }

    UnloadAllSounds(sounds);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
