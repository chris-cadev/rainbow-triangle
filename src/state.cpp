#include "state.h"
#include "colors.h"

static float ColWidth(int screenWidth)
{
    return (float)screenWidth / (float)NUM_COLORS;
}

void InitGame(GameState& state, int screenWidth, int screenHeight)
{
    state.phase            = PHASE_MENU;
    state.score            = 0;
    state.lives            = 3;
    state.columnIndex      = 3;
    state.targetColorIndex = GetRandomValue(0, NUM_COLORS - 1);
    state.bgColor          = backgroundColor;
    state.wallY            = 0.0f;
    state.wallCooldown     = 0.0f;
    state.fallSpeed        = 100.0f;
    state.acceleration     = 200.0f;
    state.currentSpeed     = state.fallSpeed;
    state.hopTimer         = 0.0f;
    state.passedColumn     = 0;
    state.passedRecorded   = false;
    state.menuSelection    = 0;
    state.difficulty       = 1;
    state.volume           = 8;
    state.editing          = false;

    (void)screenHeight;
}

static void LaunchGame(GameState& state, int screenWidth)
{
    state.phase            = PHASE_PLAY;
    state.score            = 0;
    state.lives            = 3;
    state.columnIndex      = 3;
    state.targetColorIndex = GetRandomValue(0, NUM_COLORS - 1);
    state.bgColor          = GetRainbowColor(state.targetColorIndex);
    state.wallY            = 0.0f;
    state.wallCooldown     = 0.0f;

    switch (state.difficulty)
    {
        case 0: state.fallSpeed = 60.0f;  state.acceleration = 120.0f; break;
        case 1: state.fallSpeed = 100.0f; state.acceleration = 200.0f; break;
        case 2: state.fallSpeed = 150.0f; state.acceleration = 300.0f; break;
        default: state.fallSpeed = 100.0f; state.acceleration = 200.0f; break;
    }

    state.currentSpeed     = state.fallSpeed;
    state.hopTimer         = 0.0f;
    state.passedColumn     = 0;
    state.passedRecorded   = false;
}

void UpdateGame(GameState& state, InputState input, int screenWidth, int screenHeight, float dt, const SoundBank& sounds)
{
    if (state.phase == PHASE_MENU)
    {
        enum { MAIN_PLAY, MAIN_OPTIONS, MAIN_ITEMS };

        if (input.left)  state.menuSelection = (state.menuSelection - 1 + MAIN_ITEMS) % MAIN_ITEMS;
        if (input.right) state.menuSelection = (state.menuSelection + 1) % MAIN_ITEMS;

        if (input.action)
        {
            if (state.menuSelection == MAIN_PLAY)
            {
                LaunchGame(state, screenWidth);
                float vol = state.volume / 10.0f;
                SetSoundVolume(sounds.point, vol);
                SetSoundVolume(sounds.powerup, vol);
                SetSoundVolume(sounds.powerup2, vol);
                SetSoundVolume(sounds.gameover, vol);
                SetSoundVolume(sounds.chorro, vol);
                PlaySound(sounds.powerup2);
            }
            else
            {
                state.phase = PHASE_OPTIONS;
                state.menuSelection = 0;
                state.editing = false;
            }
        }
        return;
    }

    if (state.phase == PHASE_OPTIONS)
    {
        enum { OPT_DIFFICULTY, OPT_VOLUME, OPT_BACK, OPT_ITEMS };

        if (state.editing)
        {
            if (input.left)
            {
                if (state.menuSelection == OPT_DIFFICULTY)
                    state.difficulty = (state.difficulty - 1 + 3) % 3;
                else
                    state.volume = (state.volume - 1 + 11) % 11;
            }
            if (input.right)
            {
                if (state.menuSelection == OPT_DIFFICULTY)
                    state.difficulty = (state.difficulty + 1) % 3;
                else
                    state.volume = (state.volume + 1) % 11;
            }
            if (input.action)
            {
                state.editing = false;
                PlaySound(sounds.powerup2);
            }
        }
        else
        {
            if (input.left)  state.menuSelection = (state.menuSelection - 1 + OPT_ITEMS) % OPT_ITEMS;
            if (input.right) state.menuSelection = (state.menuSelection + 1) % OPT_ITEMS;

            if (input.action)
            {
                if (state.menuSelection == OPT_BACK)
                {
                    state.phase = PHASE_MENU;
                    state.menuSelection = 0;
                }
                else
                {
                    state.editing = true;
                }
            }
        }
        return;
    }

    if (state.phase == PHASE_GAMEOVER)
    {
        if (input.action)
        {
            InitGame(state, screenWidth, screenHeight);
            PlaySound(sounds.powerup2);
        }
        return;
    }

    if (state.hopTimer > 0.0f)
        state.hopTimer -= dt;

    if (input.left)
    {
        state.columnIndex = (state.columnIndex - 1 + NUM_COLORS) % NUM_COLORS;
        state.hopTimer = 0.12f;
    }
    if (input.right)
    {
        state.columnIndex = (state.columnIndex + 1) % NUM_COLORS;
        state.hopTimer = 0.12f;
    }

    float colW = ColWidth(screenWidth);

    float cx = (state.columnIndex + 0.5f) * colW;
    float halfW = colW * 0.5f;
    float h = colW;
    float sh = (float)screenHeight;
    state.player = { { cx, sh - h }, { cx - halfW, sh }, { cx + halfW, sh } };

    if (state.wallCooldown > 0.0f)
    {
        state.wallCooldown -= dt;
        if (state.wallCooldown <= 0.0f)
        {
            state.wallY = 0.0f;
            state.currentSpeed = state.fallSpeed;
        }
        return;
    }

    state.currentSpeed += state.acceleration * dt;
    state.wallY += state.currentSpeed * dt;

    float tipX = cx;
    float tipY = sh - colW;
    float tipOffset = colW * 0.15f;

    if (!state.passedRecorded)
    {
        for (int i = 0; i < NUM_COLORS; i++)
        {
            Rectangle sq = { i * colW, state.wallY, colW, colW };
            if (tipX >= sq.x && tipX < sq.x + sq.width &&
                tipY >= sq.y - tipOffset && tipY < sq.y + sq.height + tipOffset)
            {
                state.passedColumn = i;
                state.passedRecorded = true;
                break;
            }
        }
    }

    if (state.wallY > screenHeight)
    {
        if (state.passedColumn == state.targetColorIndex)
        {
            state.score++;
            PlaySound(sounds.point);
            int prev = state.targetColorIndex;
            while (state.targetColorIndex == prev)
                state.targetColorIndex = GetRandomValue(0, NUM_COLORS - 1);
            state.bgColor = GetRainbowColor(state.targetColorIndex);
            state.fallSpeed *= 1.15f;
            if (state.fallSpeed > 600.0f)
                state.fallSpeed = 600.0f;
            if (state.score % 3 == 0)
            {
                state.acceleration *= 1.30f;
                if (state.acceleration > 900.0f)
                    state.acceleration = 900.0f;
            }
        }
        else
        {
            state.lives--;
            PlaySound(sounds.powerup);
            if (state.lives <= 0)
            {
                state.phase = PHASE_GAMEOVER;
                PlaySound(sounds.gameover);
                return;
            }
        }
        state.wallY = (float)screenHeight + colW;
        state.wallCooldown = 1.2f;
        state.passedRecorded = false;
    }
}
