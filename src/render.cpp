#include <cstdio>
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

    const char *title = "RAINBOW TRIANGLE";
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

    const char *hint = "Left/Right: navigate    Space/Enter: select";
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
        OPT_BACK,
        OPT_ITEMS
    };

    ClearBackground(backgroundColor);

    const char *title = "OPTIONS";
    float titleWidth = MeasureText(title, 50);
    DrawText(title, screenWidth / 2.0f - titleWidth / 2.0f, screenHeight * 0.15f, 50, WHITE);

    const char *difficultyLabel = TextFormat("Difficulty: %s", DifficultyLabel(state.difficultyLevel));
    const char *volumeLabel = TextFormat("Volume: %d", state.volumeLevel);
    const char *backLabel = "BACK";

    const char *items[OPT_ITEMS] = {difficultyLabel, volumeLabel, backLabel};
    float fontSize = 32.0f;
    float itemSpacing = 50.0f;
    float widths[OPT_ITEMS];
    float cursorX = CenteredStartX(screenWidth, items, OPT_ITEMS, fontSize, itemSpacing, widths);
    float cursorY = screenHeight * 0.45f;

    for (int i = 0; i < OPT_ITEMS; i++)
    {
        Color textColor;
        if (i == state.selectedMenuIndex && state.isEditing)
            textColor = GREEN;
        else if (i == state.selectedMenuIndex)
            textColor = YELLOW;
        else
            textColor = WHITE;

        DrawText(items[i], cursorX, cursorY, fontSize, textColor);
        cursorX += widths[i] + itemSpacing;
    }

    const char *hint;
    if (state.isEditing)
        hint = "Left/Right: change    Space/Enter: stop editing";
    else
        hint = "Left/Right: navigate    Space/Enter: edit / back";

    float hintWidth = MeasureText(hint, 18);
    DrawText(hint, screenWidth / 2.0f - hintWidth / 2.0f, screenHeight * 0.75f, 18, GRAY);

    return;
}

void DrawGameOver(const GameState &state, int screenWidth, int screenHeight)
{
    DrawRectangle(0, 0, screenWidth, screenHeight, {0, 0, 0, 180});

    const char *message = "GAME OVER";
    float messageWidth = MeasureText(message, 60);
    DrawText(message, screenWidth / 2.0f - messageWidth / 2.0f, screenHeight * 0.25f, 60, WHITE);

    char scoreBuf[64];
    snprintf(scoreBuf, sizeof(scoreBuf), "Score: %d", state.score);
    float scoreWidth = MeasureText(scoreBuf, 36);
    DrawText(scoreBuf, screenWidth / 2.0f - scoreWidth / 2.0f, screenHeight * 0.4f, 36, WHITE);

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

    const char *hint = "Left/Right: navigate    Space/Enter: select";
    float hintWidth = MeasureText(hint, 18);
    DrawText(hint, screenWidth / 2.0f - hintWidth / 2.0f, screenHeight * 0.7f, 18, GRAY);
}

void DrawGame(const GameState &state, int screenWidth, int screenHeight)
{
    ClearBackground(state.targetBackgroundColor);

    float columnWidth = (float)screenWidth / (float)NUM_COLORS;

    for (int i = 0; i < NUM_COLORS; i++)
    {
        Rectangle square = {i * columnWidth, state.wallTopY, columnWidth, columnWidth};
        DrawRectangleRec(square, GetRainbowColor(i));
    }

    float hopVerticalOffset = 0.0f;
    if (state.hopRemainingTime > 0.0f)
    {
        float hopProgress = state.hopRemainingTime / HOP_DURATION;
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

    if (state.phase == PHASE_GAMEOVER)
    {
        DrawGameOver(state, screenWidth, screenHeight);
        return;
    }

    DrawGame(state, screenWidth, screenHeight);
}
