#include <cstdio>
#include <math.h>
#include "render.h"
#include "colors.h"
#include "constants.h"
#include "state.h"

static const char *DifficultyLabel(int difficultyLevel)
{
    const char *labels[] = {"Easy", "Medium", "Hard"};
    return labels[difficultyLevel % 3];
}

// Measures text items and returns the starting X for a centered horizontal layout
static float CenteredStartX(int screenWidth, const char* items[], int count, float fontSize, float spacing, float outWidths[])
{
    float totalWidth = 0.0f;
    for (int i = 0; i < count; i++)
    {
        outWidths[i] = MeasureText(items[i], fontSize);
        totalWidth += outWidths[i];
    }
    totalWidth += spacing * (count - 1);
    return screenWidth / 2.0f - totalWidth / 2.0f;
}

void DrawMenu(const GameState &state, int screenWidth, int screenHeight)
{
    enum
    {
        MAIN_PLAY,
        MAIN_OPTIONS,
        MAIN_ITEMS
    };

    ClearBackground(backgroundColor);

    const char *title = "RAINBOW\nTRIANGLE";
    float titleWidth = MeasureText(title, 60);
    DrawText(title, screenWidth / 2.0f - titleWidth / 2.0f, screenHeight * 0.25f, 60, WHITE);

    const char *items[MAIN_ITEMS] = {"PLAY", "OPTIONS"};
    float fontSize = 36.0f;
    float itemSpacing = 80.0f;
    float widths[MAIN_ITEMS];
    float cursorX = CenteredStartX(screenWidth, items, MAIN_ITEMS, fontSize, itemSpacing, widths);
    float cursorY = screenHeight * 0.5f;

    for (int i = 0; i < MAIN_ITEMS; i++)
    {
        Color textColor = (i == state.selectedMenuIndex) ? YELLOW : WHITE;
        DrawText(items[i], cursorX, cursorY, fontSize, textColor);
        cursorX += widths[i] + itemSpacing;
    }

    const char *hint = "Left/Right: navigate    Click/Space/Enter: select";
    float hintWidth = MeasureText(hint, 18);
    DrawText(hint, screenWidth / 2.0f - hintWidth / 2.0f, screenHeight * 0.7f, 18, GRAY);

    return;
}

void DrawOptions(const GameState &state, int screenWidth, int screenHeight)
{
    enum
    {
        OPT_DIFFICULTY,
        OPT_VOLUME,
        OPT_SAFE_MODE,
        OPT_MUSIC,
        OPT_MUSIC_VOLUME,
        OPT_BACK,
        OPT_ITEMS
    };

    ClearBackground(backgroundColor);

    const char *title = "OPTIONS";
    float titleWidth = MeasureText(title, 50);
    DrawText(title, screenWidth / 2.0f - titleWidth / 2.0f, screenHeight * 0.12f, 50, WHITE);

    const char *difficultyLabel = TextFormat("Difficulty: %s", DifficultyLabel(state.difficultyLevel));
    const char *volumeLabel = TextFormat("Volume: %d", state.volumeLevel);
    const char *safeLabel = state.safeMode ? "Safe Mode: On" : "Safe Mode: Off";
    const char *musicLabel = state.musicEnabled ? "Music: On" : "Music: Off";
    const char *musicVolLabel = TextFormat("Music Vol: %d", state.musicLevel);
    const char *backLabel = "BACK";

    const char *items[OPT_ITEMS] = {difficultyLabel, volumeLabel, safeLabel, musicLabel, musicVolLabel, backLabel};
    float fontSize = 28.0f;
    float itemSpacing = 40.0f;
    float widths[OPT_ITEMS];
    CenteredStartX(screenWidth, items, OPT_ITEMS, fontSize, itemSpacing, widths);
    float cursorX;
    float cursorY = screenHeight * 0.35f;

    for (int i = 0; i < OPT_ITEMS; i++)
    {
        Color textColor;
        if (i == state.selectedMenuIndex && state.isEditing)
            textColor = GREEN;
        else if (i == state.selectedMenuIndex)
            textColor = YELLOW;
        else
            textColor = WHITE;

        cursorX = screenWidth / 2.0f - widths[i] / 2.0f;
        DrawText(items[i], cursorX, cursorY, fontSize, textColor);
        cursorY += fontSize + itemSpacing;
    }

    const char *hint;
    if (state.isEditing)
        hint = "Left/Right: change    Click/Space/Enter: stop editing";
    else
        hint = "Left/Right: navigate    Click/Space/Enter: edit / back";

    float hintWidth = MeasureText(hint, 18);
    DrawText(hint, screenWidth / 2.0f - hintWidth / 2.0f, screenHeight * 0.75f, 18, GRAY);

    return;
}

void DrawWarning(const GameState &state, int screenWidth, int screenHeight)
{
    ClearBackground(backgroundColor);

    DrawRectangle(0, 0, screenWidth, screenHeight, {0, 0, 0, 200});

    const char *lines[] = {
        "PRECAUTION",
        "This game contains visual effects",
        "and color changes that may affect",
        "people with photosensitive epilepsy."
    };
    float y = screenHeight * 0.2f;
    for (int i = 0; i < 3; i++)
    {
        float w = MeasureText(lines[i], i == 0 ? 40 : 20);
        DrawText(lines[i], screenWidth / 2.0f - w / 2.0f, y, i == 0 ? 40 : 20, WHITE);
        y += i == 0 ? 50 : 30;
    }

    enum { WARN_CONTINUE, WARN_SAFE, WARN_ITEMS };
    const char *warnItems[WARN_ITEMS] = {"Continue", "Activate Safe Mode"};
    float fontSize = 28.0f;
    float itemSpacing = 60.0f;
    float widths[WARN_ITEMS];
    float cursorX = CenteredStartX(screenWidth, warnItems, WARN_ITEMS, fontSize, itemSpacing, widths);
    float cursorY = screenHeight * 0.6f;

    for (int i = 0; i < WARN_ITEMS; i++)
    {
        Color textColor = (i == state.warningSelectedIndex) ? YELLOW : WHITE;
        DrawText(warnItems[i], cursorX, cursorY, fontSize, textColor);
        cursorX += widths[i] + itemSpacing;
    }

    const char *hint = "Left/Right: navigate    Click/Space/Enter: select";
    float hintWidth = MeasureText(hint, 18);
    DrawText(hint, screenWidth / 2.0f - hintWidth / 2.0f, screenHeight * 0.75f, 18, GRAY);
}

void DrawGameOver(const GameState &state, int screenWidth, int screenHeight)
{
    DrawRectangle(0, 0, screenWidth, screenHeight, {0, 0, 0, 180});

    if (state.goStage == 0)
    {
        Vector2 top = state.player.a;
        Vector2 bl = state.player.b;
        Vector2 br = state.player.c;
        DrawTriangle(top, bl, br, WHITE);

        if (state.goTimer >= GO_CRACK_TIME)
        {
            Vector2 center = TriangleCenter(state.player);
            for (int i = 0; i < 8; i++)
            {
                int h = i * 104729 + 42;
                float baseAngle = ((h % 1000) / 1000.0f) * 2.0f * PI;
                Vector2 prev = center;
                int segments = 2 + (h / 1000) % 3;
                float totalLen = 15.0f + ((h / 1000 / 3) % 1000 / 1000.0f) * 50.0f;
                float segLen = totalLen / segments;
                float angle = baseAngle;

                for (int j = 0; j < segments; j++)
                {
                    angle += ((j % 2 == 0) ? 1 : -1) * 0.4f;
                    Vector2 next = {prev.x + cosf(angle) * segLen, prev.y + sinf(angle) * segLen};
                    DrawLineEx(prev, next, 2.0f, {20, 20, 20, 255});
                    prev = next;
                }
            }
        }
    }

    const char *message = "GAME\nOVER";
    float messageWidth = MeasureText(message, 60);
    DrawText(message, screenWidth / 2.0f - messageWidth / 2.0f, screenHeight * 0.25f, 60, WHITE);

    char scoreBuf[64];
    snprintf(scoreBuf, sizeof(scoreBuf), "Score: %d", state.score);
    float scoreWidth = MeasureText(scoreBuf, 36);
    DrawText(scoreBuf, screenWidth / 2.0f - scoreWidth / 2.0f, screenHeight * 0.4f, 36, WHITE);

    if (state.goStage >= 1)
    {
        for (int i = 0; i < MAX_GO_PARTICLES; i++)
        {
            const GoParticle &p = state.goParticles[i];
            if (!p.active) continue;

            float alpha = p.life < 1.0f ? p.life : 1.0f;
            Color color = {255, 255, 255, (unsigned char)(alpha * 255)};

            float h = p.size * 0.5f;
            float w = p.size * 0.5f;
            float c = cosf(p.rotation);
            float s = sinf(p.rotation);

            Vector2 v0 = {p.pos.x + h * s,         p.pos.y - h * c};
            Vector2 v1 = {p.pos.x - w * c - h * s, p.pos.y - w * s + h * c};
            Vector2 v2 = {p.pos.x + w * c - h * s, p.pos.y + w * s + h * c};

            DrawTriangle(v0, v1, v2, color);
        }
    }

    if (state.goStage == 2)
    {
        const char *items[] = {"RETRY", "MENU"};
        float fontSize = 36.0f;
        float itemSpacing = 80.0f;
        float widths[2];
        float cursorX = CenteredStartX(screenWidth, items, 2, fontSize, itemSpacing, widths);
        float cursorY = screenHeight * 0.55f;

        for (int i = 0; i < 2; i++)
        {
            Color textColor = (i == state.selectedMenuIndex) ? YELLOW : WHITE;
            DrawText(items[i], cursorX, cursorY, fontSize, textColor);
            cursorX += widths[i] + itemSpacing;
        }

        const char *hint = "Left/Right: navigate    Click/Space/Enter: select";
        float hintWidth = MeasureText(hint, 18);
        DrawText(hint, screenWidth / 2.0f - hintWidth / 2.0f, screenHeight * 0.7f, 18, GRAY);
    }
}

void DrawGame(const GameState &state, int screenWidth, int screenHeight)
{
    ClearBackground(state.displayBackgroundColor);

    float columnWidth = (float)screenWidth / (float)NUM_COLORS;

    for (int i = 0; i < NUM_COLORS; i++)
    {
        Rectangle square = {i * columnWidth, state.wallTopY, columnWidth, columnWidth};
        Color wallColor = state.safeMode ? GetSafeRainbowColor(i) : GetRainbowColor(i);
        DrawRectangleRec(square, wallColor);
    }

    float hopVerticalOffset = 0.0f;
    if (state.triangleHopRemainingTime > 0.0f)
    {
        float hopProgress = state.triangleHopRemainingTime / HOP_DURATION;
        hopVerticalOffset = -HOP_HEIGHT * 4.0f * hopProgress * (1.0f - hopProgress);
    }

    Vector2 topVertex = {state.player.a.x, state.player.a.y + hopVerticalOffset};
    Vector2 bottomLeftVertex = {state.player.b.x, state.player.b.y + hopVerticalOffset};
    Vector2 bottomRightVertex = {state.player.c.x, state.player.c.y + hopVerticalOffset};

    DrawTriangle(topVertex, bottomLeftVertex, bottomRightVertex, backgroundColor);

    char scoreBuf[32];
    snprintf(scoreBuf, sizeof(scoreBuf), "Score: %d", state.score);
    DrawText(scoreBuf, 10, 10, 28, WHITE);

    // Measure worst-case width so the label position stays fixed as lives change
    float livesLabelWidth = MeasureText("Lives: 99", 28);
    char livesBuf[32];
    snprintf(livesBuf, sizeof(livesBuf), "Lives: %d", state.lives);
    DrawText(livesBuf, screenWidth - livesLabelWidth - 10, 10, 28, WHITE);
}

void DrawScene(const GameState &state, int screenWidth, int screenHeight)
{
    if (state.phase == PHASE_MENU)
    {
        DrawMenu(state, screenWidth, screenHeight);
        return;
    }

    if (state.phase == PHASE_OPTIONS)
    {
        DrawOptions(state, screenWidth, screenHeight);
        return;
    }

    if (state.phase == PHASE_WARNING)
    {
        DrawWarning(state, screenWidth, screenHeight);
        return;
    }

    if (state.phase == PHASE_GAMEOVER)
    {
        DrawGameOver(state, screenWidth, screenHeight);
        return;
    }

    DrawGame(state, screenWidth, screenHeight);
}
