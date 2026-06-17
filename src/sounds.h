#pragma once
#include "raylib.h"

static const char* const bgmMusic   = "src/assets/music.ogg";

static const char* const sfxGameover = "src/assets/gameover.mp3";
static const char* const sfxPoint    = "src/assets/point.mp3";

static const char* const sfxLostLife[] = {
    "src/assets/lost_life_1.mp3",
    "src/assets/lost_life_2.ogg",
    "src/assets/lost_life_3.mp3",
    "src/assets/lost_life_4.mp3"
};
static constexpr int NUM_LOST_LIFE_SOUNDS = 4;

static const char* const sfxGameoverOnLessThan3[] = {
    "src/assets/gameover_on_less_than_3_points_1.mp3",
    "src/assets/gameover_on_less_than_3_points_2.mp3",
    "src/assets/gameover_on_less_than_3_points_3.mp3",
    "src/assets/gameover_on_less_than_3_points_4.mp3"
};
static constexpr int NUM_GAMEOVER_ON_LESS_THAN_3_SOUNDS = 4;

struct SoundBank
{
    Sound gameover;
    Sound point;
    Sound lostLife[NUM_LOST_LIFE_SOUNDS];
    Sound gameoverOnLessThan3[NUM_GAMEOVER_ON_LESS_THAN_3_SOUNDS];
};

inline SoundBank LoadAllSounds()
{
    SoundBank bank;
    bank.gameover = LoadSound(sfxGameover);
    bank.point    = LoadSound(sfxPoint);
    for (int i = 0; i < NUM_LOST_LIFE_SOUNDS; i++)
        bank.lostLife[i] = LoadSound(sfxLostLife[i]);
    for (int i = 0; i < NUM_GAMEOVER_ON_LESS_THAN_3_SOUNDS; i++)
        bank.gameoverOnLessThan3[i] = LoadSound(sfxGameoverOnLessThan3[i]);
    return bank;
}

inline void UnloadAllSounds(SoundBank bank)
{
    UnloadSound(bank.gameover);
    UnloadSound(bank.point);
    for (int i = 0; i < NUM_LOST_LIFE_SOUNDS; i++)
        UnloadSound(bank.lostLife[i]);
    for (int i = 0; i < NUM_GAMEOVER_ON_LESS_THAN_3_SOUNDS; i++)
        UnloadSound(bank.gameoverOnLessThan3[i]);
}
