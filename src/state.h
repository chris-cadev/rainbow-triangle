#pragma once
#include "raylib.h"
#include "collider.h"
#include "input.h"
#include "sounds.h"

enum GamePhase
{
    PHASE_MENU,
    PHASE_OPTIONS,
    PHASE_WARNING,
    PHASE_PLAYING,
    PHASE_GAMEOVER
};

enum { NUM_COLORS = 7 };

struct GoParticle
{
    Vector2 pos;
    Vector2 vel;
    float   size;
    float   rotation;
    float   rotSpeed;
    float   life;
    bool    active;
};

constexpr int MAX_GO_PARTICLES = 100;

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
    float      triangleHopRemainingTime;
    int        passedColumn;
    bool       hasPassedWall;
    int        selectedMenuIndex;
    int        difficultyLevel;
    int        volumeLevel;
    bool       isEditing;
    bool       safeMode;
    Color      displayBackgroundColor;
    bool       warningShown;
    bool       musicEnabled;
    int        musicLevel;
    int        warningSelectedIndex;
    Music      gameMusic;
    bool       soundsLoaded;
    int        lastLostLifeIndex;
    float      goTimer;
    float      goElapsed;
    int        goStage;
    GoParticle goParticles[MAX_GO_PARTICLES];
};

void InitGame(GameState& state);
void UpdateGame(GameState& state, InputState input, int screenWidth, int screenHeight, float deltaTime, const SoundBank& sounds);
