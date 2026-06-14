#pragma once
#include "raylib.h"

static const char* const sfxChorro   = "src/assets/chorro.mp3";
static const char* const sfxGameover = "src/assets/gameover.mp3";
static const char* const sfxPoint    = "src/assets/point.mp3";
static const char* const sfxPowerup  = "src/assets/powerup.mp3";
static const char* const sfxPowerup2 = "src/assets/powerup2.wav";

struct SoundBank
{
    Sound chorro;
    Sound gameover;
    Sound point;
    Sound powerup;
    Sound powerup2;
};

inline SoundBank LoadAllSounds()
{
    SoundBank b;
    b.chorro   = LoadSound(sfxChorro);
    b.gameover = LoadSound(sfxGameover);
    b.point    = LoadSound(sfxPoint);
    b.powerup  = LoadSound(sfxPowerup);
    b.powerup2 = LoadSound(sfxPowerup2);
    return b;
}

inline void UnloadAllSounds(SoundBank b)
{
    UnloadSound(b.chorro);
    UnloadSound(b.gameover);
    UnloadSound(b.point);
    UnloadSound(b.powerup);
    UnloadSound(b.powerup2);
}
