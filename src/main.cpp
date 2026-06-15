#include "raylib.h"
#include "colors.h"
#include "constants.h"
#include "render.h"
#include "state.h"
#include "input.h"
#include "sounds.h"

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(INITIAL_WIDTH, INITIAL_WIDTH * ASPECT_DIVISOR, "Rainbow Triangle");
    InitAudioDevice();
    SetTargetFPS(TARGET_FPS);

    int maxMonitorWidth = GetMonitorWidth(GetCurrentMonitor());

    SoundBank sounds = {};
    bool soundsLoaded = false;

    GameState state;
    InitGame(state);

    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();
        if (deltaTime > MAX_FRAME_DELTA) deltaTime = MAX_FRAME_DELTA;
        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();

        if (IsWindowResized())
        {
            int newScreenHeight = screenHeight;
            int newScreenWidth = (int)((float)screenHeight / ASPECT_DIVISOR + 0.5f);
            if (newScreenWidth > maxMonitorWidth)
            {
                newScreenWidth = maxMonitorWidth;
                newScreenHeight = maxMonitorWidth * ASPECT_DIVISOR;
            }
            if (newScreenWidth != screenWidth || newScreenHeight != screenHeight)
                SetWindowSize(newScreenWidth, newScreenHeight);
            screenWidth = newScreenWidth;
            screenHeight = newScreenHeight;
        }

        int innerWidth = screenWidth;
        int innerHeight = screenHeight;

        InputState input = GetInput();

        if (!soundsLoaded)
        {
            if (input.action || input.left || input.right ||
                input.pointerClicked || input.pointerRightClicked)
            {
                sounds = LoadAllSounds();
                {
                    float volumeFraction = state.volumeLevel / 10.0f;
                    SetSoundVolume(sounds.point, volumeFraction);
                    SetSoundVolume(sounds.gameover, volumeFraction);
                    for (int i = 0; i < 4; i++)
                        SetSoundVolume(sounds.lostLife[i], volumeFraction);
                    for (int i = 0; i < 2; i++)
                        SetSoundVolume(sounds.gameoverOnLessThan3[i], volumeFraction);
                }
                state.soundsLoaded = true;
                soundsLoaded = true;
            }
        }

        UpdateGame(state, input, innerWidth, innerHeight, deltaTime, sounds);

        BeginDrawing();
        ClearBackground(BLACK);
        DrawScene(state, innerWidth, innerHeight);
        EndDrawing();
    }

    if (soundsLoaded)
        UnloadAllSounds(sounds);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
