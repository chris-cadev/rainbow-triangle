#include <algorithm>
#include "state.h"
#include "colors.h"
#include "constants.h"

static float ColumnWidth(int screenWidth)
{
    return (float)screenWidth / (float)NUM_COLORS;
}

static void ResetGameplayState(GameState &state)
{
    state.score = 0;
    state.lives = 3;
    state.columnIndex = 3;
    int minTarget = std::max(0, state.columnIndex - 3);
    int maxTarget = std::min(NUM_COLORS - 1, state.columnIndex + 3);
    state.targetColorIndex = GetRandomValue(minTarget, maxTarget);
    state.wallTopY = 0.0f;
    state.wallCooldownTimer = 0.0f;
    state.currentSpeed = state.fallSpeed;
    state.hopRemainingTime = 0.0f;
    state.passedColumn = 0;
    state.hasPassedWall = false;
    state.lastLostLifeIndex = -1;
}

void InitGame(GameState &state)
{
    state.phase = PHASE_MENU;
    state.fallSpeed = 100.0f;
    state.acceleration  = 200.0f;
    state.targetBackgroundColor = backgroundColor;
    state.selectedMenuIndex = 0;
    state.difficultyLevel   = 1;
    state.volumeLevel   = 8;
    state.isEditing = false;
    ResetGameplayState(state);
}

static void LaunchGame(GameState &state, int screenWidth)
{
    state.phase = PHASE_PLAY;

    static const float difficultyFallSpeeds[] = {0.075f, 0.125f, 0.1875f};
    static const float difficultyAccelerations[] = {0.15f, 0.25f, 0.375f};
    int difficultyIndex = state.difficultyLevel;
    state.fallSpeed = difficultyFallSpeeds[difficultyIndex];
    state.acceleration = difficultyAccelerations[difficultyIndex];

    ResetGameplayState(state);
    state.targetBackgroundColor = GetRainbowColor(state.targetColorIndex);
}

void UpdateGame(GameState &state, InputState input, int screenWidth, int screenHeight, float deltaTime, const SoundBank &sounds)
{
    if (state.phase == PHASE_MENU)
    {
        enum
        {
            MAIN_PLAY,
            MAIN_OPTIONS,
            MAIN_ITEMS
        };

        if (input.left)
            state.selectedMenuIndex = (state.selectedMenuIndex - 1 + MAIN_ITEMS) % MAIN_ITEMS;
        if (input.right)
            state.selectedMenuIndex = (state.selectedMenuIndex + 1) % MAIN_ITEMS;

        if (input.action)
        {
            if (state.selectedMenuIndex == MAIN_PLAY)
            {
                LaunchGame(state, screenWidth);
                float volumeFraction = state.volumeLevel / 10.0f;
                SetSoundVolume(sounds.point, volumeFraction);
                SetSoundVolume(sounds.gameover, volumeFraction);
                for (int i = 0; i < 4; i++)
                    SetSoundVolume(sounds.lostLife[i], volumeFraction);
                for (int i = 0; i < 2; i++)
                    SetSoundVolume(sounds.gameoverOnLessThan3[i], volumeFraction);
            }
            else
            {
                state.phase = PHASE_OPTIONS;
                state.selectedMenuIndex = 0;
                state.isEditing = false;
            }
        }
        return;
    }

    if (state.phase == PHASE_OPTIONS)
    {
        enum
        {
            OPT_DIFFICULTY,
            OPT_VOLUME,
            OPT_BACK,
            OPT_ITEMS
        };
        if (state.isEditing)
        {
            if (input.left)
            {
                if (state.selectedMenuIndex == OPT_DIFFICULTY)
                    state.difficultyLevel = (state.difficultyLevel - 1 + 3) % 3;
                else
                    state.volumeLevel = (state.volumeLevel - 1 + 11) % 11;
            }
            if (input.right)
            {
                if (state.selectedMenuIndex == OPT_DIFFICULTY)
                    state.difficultyLevel = (state.difficultyLevel + 1) % 3;
                else
                    state.volumeLevel = (state.volumeLevel + 1) % 11;
            }
            if (input.action)
            {
                state.isEditing = false;
            }
        }
        else
        {
            if (input.left)
                state.selectedMenuIndex = (state.selectedMenuIndex - 1 + OPT_ITEMS) % OPT_ITEMS;
            if (input.right)
                state.selectedMenuIndex = (state.selectedMenuIndex + 1) % OPT_ITEMS;

            if (input.action)
            {
                if (state.selectedMenuIndex == OPT_BACK)
                {
                    state.phase = PHASE_MENU;
                    state.selectedMenuIndex = 0;
                }
                else
                {
                    state.isEditing = true;
                }
            }
        }
        return;
    }

    if (state.phase == PHASE_GAMEOVER)
    {
        enum { GO_RETRY, GO_MENU, GO_ITEMS };

        if (input.left)
            state.selectedMenuIndex = (state.selectedMenuIndex - 1 + GO_ITEMS) % GO_ITEMS;
        if (input.right)
            state.selectedMenuIndex = (state.selectedMenuIndex + 1) % GO_ITEMS;

        if (input.action)
        {
            if (state.selectedMenuIndex == GO_RETRY)
            {
                LaunchGame(state, screenWidth);
                float volumeFraction = state.volumeLevel / 10.0f;
                SetSoundVolume(sounds.point, volumeFraction);
                SetSoundVolume(sounds.gameover, volumeFraction);
                for (int i = 0; i < 4; i++)
                    SetSoundVolume(sounds.lostLife[i], volumeFraction);
                for (int i = 0; i < 2; i++)
                    SetSoundVolume(sounds.gameoverOnLessThan3[i], volumeFraction);
            }
            else
            {
                InitGame(state);
            }
        }
        return;
    }

    if (state.hopRemainingTime > 0.0f)
        state.hopRemainingTime -= deltaTime;

    if (input.left)
    {
        state.columnIndex = (state.columnIndex - 1 + NUM_COLORS) % NUM_COLORS;
        state.hopRemainingTime = HOP_DURATION;
    }
    if (input.right)
    {
        state.columnIndex = (state.columnIndex + 1) % NUM_COLORS;
        state.hopRemainingTime = HOP_DURATION;
    }

    float columnWidth = ColumnWidth(screenWidth);

    float centerX = (state.columnIndex + 0.5f) * columnWidth;
    float halfWidth = columnWidth * 0.5f;
    float triangleHeight = columnWidth;
    float screenHeightFloat = (float)screenHeight;
    state.player = {
        {centerX, screenHeightFloat - triangleHeight},
        {centerX - halfWidth, screenHeightFloat},
        {centerX + halfWidth, screenHeightFloat}};

    state.currentSpeed += state.acceleration * deltaTime;
    state.wallTopY += state.currentSpeed * screenHeightFloat * deltaTime;

    float playerTipX = centerX;
    float playerTipY = screenHeightFloat - triangleHeight;
    float collisionTolerance = columnWidth * TIP_TOLERANCE;

    if (!state.hasPassedWall)
    {
        int hitColumn = (int)(playerTipX / columnWidth);
        if (hitColumn >= 0 && hitColumn < NUM_COLORS &&
            playerTipY >= state.wallTopY - collisionTolerance &&
            playerTipY < state.wallTopY + columnWidth + collisionTolerance)
        {
            state.passedColumn = hitColumn;
            state.hasPassedWall = true;
        }
    }

    if (state.wallTopY > screenHeight)
    {
        if (state.passedColumn == state.targetColorIndex)
        {
            state.score++;
            PlaySound(sounds.point);
            int previousColorIndex = state.targetColorIndex;
            int minTarget = std::max(0, state.columnIndex - 3);
            int maxTarget = std::min(NUM_COLORS - 1, state.columnIndex + 3);
            do
                state.targetColorIndex = GetRandomValue(minTarget, maxTarget);
            while (state.targetColorIndex == previousColorIndex && maxTarget > minTarget);
            state.targetBackgroundColor = GetRainbowColor(state.targetColorIndex);
            state.fallSpeed *= SPEED_SCALE;
            if (state.fallSpeed > MAX_FALL_SPEED)
                state.fallSpeed = MAX_FALL_SPEED;
            if (state.score % ACCEL_INTERVAL == 0)
            {
                state.acceleration *= ACCEL_SCALE;
                if (state.acceleration > MAX_ACCEL)
                    state.acceleration = MAX_ACCEL;
            }
        }
        else
        {
            state.lives--;
            int lostLifeIndex = GetRandomValue(0, 3);
            if (lostLifeIndex == state.lastLostLifeIndex)
                lostLifeIndex = (lostLifeIndex + 1) % 4;
            state.lastLostLifeIndex = lostLifeIndex;
            PlaySound(sounds.lostLife[lostLifeIndex]);
            if (state.lives <= 0)
            {
                state.phase = PHASE_GAMEOVER;
                if (state.score < 3)
                    PlaySound(sounds.gameoverOnLessThan3[GetRandomValue(0, 1)]);
                else
                    PlaySound(sounds.gameover);
                return;
            }
        }
        state.wallTopY = -columnWidth * 2.0f;
        state.currentSpeed = state.fallSpeed;
        state.hasPassedWall = false;
    }
}
