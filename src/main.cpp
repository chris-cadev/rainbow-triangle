#include "raylib.h"
#include "colors.h"
#include "constants.h"
#include "render.h"
#include "state.h"
#include "input.h"
#include "sounds.h"
#include "config.h"

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(INITIAL_WIDTH, INITIAL_WIDTH * ASPECT_DIVISOR, "Rainbow Triangle");
    InitAudioDevice();
    #ifdef __EMSCRIPTEN__
    {
        EM_ASM({
            document.addEventListener('keydown', function(e) {
                if (typeof window.miniaudio !== 'undefined' && window.miniaudio.unlock) {
                    window.miniaudio.unlock();
                }
            }, {once: true, capture: true});
        });
    }
    #endif
    SetTargetFPS(TARGET_FPS);

    int maxMonitorWidth = GetMonitorWidth(GetCurrentMonitor());

    SoundBank sounds = {};
    bool soundsLoaded = false;

    GameState state;
    InitGame(state);
    state.soundsLoaded = false;
    LoadConfig(state);

    bool musicLoaded = false;

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
                    for (int i = 0; i < NUM_LOST_LIFE_SOUNDS; i++)
                        SetSoundVolume(sounds.lostLife[i], volumeFraction);
                    for (int i = 0; i < NUM_GAMEOVER_ON_LESS_THAN_3_SOUNDS; i++)
                        SetSoundVolume(sounds.gameoverOnLessThan3[i], volumeFraction);
                }
                state.gameMusic = LoadMusicStream(bgmMusic);
                state.gameMusic.looping = true;
                PlayMusicStream(state.gameMusic);
                musicLoaded = true;
                state.soundsLoaded = true;
                soundsLoaded = true;
            }
        }

        if (musicLoaded)
        {
            float pitch = 0.8f;
            if (state.phase == PHASE_PLAYING)
            {
                pitch = 0.8f + (state.score / 100.0f);
                if (pitch > 1.5f) pitch = 1.5f;
                ResumeMusicStream(state.gameMusic);
            }
            else if (state.phase == PHASE_GAMEOVER)
            {
                PauseMusicStream(state.gameMusic);
            }
            else
            {
                ResumeMusicStream(state.gameMusic);
            }
            SetMusicPitch(state.gameMusic, pitch);
            float vol = state.musicEnabled ? (state.musicLevel / 10.0f) : 0.0f;
            SetMusicVolume(state.gameMusic, vol);
            UpdateMusicStream(state.gameMusic);
        }

        UpdateGame(state, input, innerWidth, innerHeight, deltaTime, sounds);

        BeginDrawing();
        ClearBackground(BLACK);
        DrawScene(state, innerWidth, innerHeight);
        EndDrawing();
    }

    if (musicLoaded)
        UnloadMusicStream(state.gameMusic);
    if (soundsLoaded)
        UnloadAllSounds(sounds);
    SaveConfig(state);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
