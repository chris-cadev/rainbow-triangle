#pragma once
#include "raylib.h"
#include "collider.h"
#include "input.h"
#include "sounds.h"

enum GamePhase
{
    PHASE_MENU,
    PHASE_OPTIONS,
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
    Color      targetBackgroundColor;
    float      wallTopY;
    float      wallCooldownTimer;
    float      fallSpeed;
    float      acceleration;
    float      currentSpeed;
    float      hopRemainingTime;
    int        passedColumn;
    bool       hasPassedWall;
    int        selectedMenuIndex;
    int        difficultyLevel;
    int        volumeLevel;
    bool       isEditing;
    int        lastLostLifeIndex;
};

void InitGame(GameState& state);
void UpdateGame(GameState& state, InputState input, int screenWidth, int screenHeight, float deltaTime, const SoundBank& sounds);
