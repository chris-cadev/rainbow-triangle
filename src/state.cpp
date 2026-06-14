#include "state.h"
#include "colors.h"

static float ColWidth(int screenWidth)
{
    return (float)screenWidth / (float)NUM_COLORS;
}

void InitGame(GameState& state, int screenWidth, int screenHeight)
{
    state.phase            = PHASE_PLAY;
    state.score            = 0;
    state.lives            = 3;
    state.columnIndex      = 3;
    state.targetColorIndex = GetRandomValue(0, NUM_COLORS - 1);
    state.bgColor          = GetRainbowColor(state.targetColorIndex);
    state.wallY            = -ColWidth(screenWidth);
    state.wallCooldown     = 0.0f;
    state.fallSpeed        = 240.0f;
    state.passedColumn     = 0;
    state.passedRecorded   = false;

    (void)screenHeight;
}

void UpdateGame(GameState& state, InputState input, int screenWidth, int screenHeight, float dt, const SoundBank& sounds)
{
    if (state.phase == PHASE_GAMEOVER)
    {
        if (input.action)
        {
            InitGame(state, screenWidth, screenHeight);
            PlaySound(sounds.powerup2);
        }
        return;
    }

    if (input.left)
        state.columnIndex = (state.columnIndex - 1 + NUM_COLORS) % NUM_COLORS;
    if (input.right)
        state.columnIndex = (state.columnIndex + 1) % NUM_COLORS;

    float colW = ColWidth(screenWidth);

    // Upward-pointing triangle, full column width, bottom flush with screen edge
    float cx = (state.columnIndex + 0.5f) * colW;
    float halfW = colW * 0.5f;
    float h = colW;
    float sh = (float)screenHeight;
    state.player = { { cx, sh - h }, { cx - halfW, sh }, { cx + halfW, sh } };

    // Wall cooldown — hidden pause between rows
    if (state.wallCooldown > 0.0f)
    {
        state.wallCooldown -= dt;
        if (state.wallCooldown <= 0.0f)
            state.wallY = -colW;
        return;
    }

    // Fall the wall
    state.wallY += state.fallSpeed * dt;

    // Detect pass using the triangle's tip point, with HCI forgiveness offset
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

    // Evaluate when wall exits the bottom of the viewport
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
            state.fallSpeed *= 1.06f;
            if (state.fallSpeed > 600.0f)
                state.fallSpeed = 600.0f;
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
