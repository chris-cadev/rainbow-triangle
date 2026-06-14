#pragma once
#include "raylib.h"
#include "collider.h"
#include "input.h"
#include "sounds.h"

enum GamePhase
{
    PHASE_PLAY,
    PHASE_GAMEOVER
};

enum { NUM_COLORS = 7 };

struct GameState
{
    GamePhase phase;
    Triangle   player;
    int        score;
    int        lives;
    int        columnIndex;
    int        targetColorIndex;
    Color      bgColor;
    float      wallY;
    float      wallCooldown;
    float      fallSpeed;
    float      rowGravity;
    float      currentSpeed;
    float      hopTimer;
    int        passedColumn;
    bool       passedRecorded;
};

void InitGame(GameState& state, int screenWidth, int screenHeight);
void UpdateGame(GameState& state, InputState input, int screenWidth, int screenHeight, float dt, const SoundBank& sounds);
