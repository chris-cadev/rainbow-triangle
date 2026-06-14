#include "render.h"
#include "colors.h"
#include "state.h"

static const char* DifficultyLabel(int d)
{
    const char* labels[] = { "Easy", "Medium", "Hard" };
    return labels[d % 3];
}

void DrawScene(const GameState& state, int screenWidth, int screenHeight)
{
    if (state.phase == PHASE_MENU)
    {
        enum { MAIN_PLAY, MAIN_OPTIONS, MAIN_ITEMS };

        ClearBackground(backgroundColor);

        const char* title = "RAINBOW TRIANGLE";
        float tw = MeasureText(title, 60);
        DrawText(title, screenWidth / 2.0f - tw / 2.0f, screenHeight * 0.25f, 60, WHITE);

        const char* items[MAIN_ITEMS] = { "PLAY", "OPTIONS" };
        float fs = 36.0f;
        float widths[MAIN_ITEMS];
        float totalW = 0.0f;
        float spacing = 80.0f;

        for (int i = 0; i < MAIN_ITEMS; i++)
        {
            widths[i] = MeasureText(items[i], fs);
            totalW += widths[i];
        }
        totalW += spacing * (MAIN_ITEMS - 1);

        float x = screenWidth / 2.0f - totalW / 2.0f;
        float y = screenHeight * 0.5f;

        for (int i = 0; i < MAIN_ITEMS; i++)
        {
            Color c = (i == state.menuSelection) ? YELLOW : WHITE;
            DrawText(items[i], x, y, fs, c);
            x += widths[i] + spacing;
        }

        const char* hint = "Left/Right: navigate    Space/Enter: select";
        float hw = MeasureText(hint, 18);
        DrawText(hint, screenWidth / 2.0f - hw / 2.0f, screenHeight * 0.7f, 18, GRAY);

        return;
    }

    if (state.phase == PHASE_OPTIONS)
    {
        enum { OPT_DIFFICULTY, OPT_VOLUME, OPT_BACK, OPT_ITEMS };

        ClearBackground(backgroundColor);

        const char* title = "OPTIONS";
        float tw = MeasureText(title, 50);
        DrawText(title, screenWidth / 2.0f - tw / 2.0f, screenHeight * 0.15f, 50, WHITE);

        const char* diffLabel = TextFormat("Difficulty: %s", DifficultyLabel(state.difficulty));
        const char* volLabel  = TextFormat("Volume: %d", state.volume);
        const char* backLabel = "BACK";

        const char* items[OPT_ITEMS] = { diffLabel, volLabel, backLabel };
        float fs = 32.0f;
        float widths[OPT_ITEMS];
        float totalW = 0.0f;
        float spacing = 50.0f;

        for (int i = 0; i < OPT_ITEMS; i++)
        {
            widths[i] = MeasureText(items[i], fs);
            totalW += widths[i];
        }
        totalW += spacing * (OPT_ITEMS - 1);

        float x = screenWidth / 2.0f - totalW / 2.0f;
        float y = screenHeight * 0.45f;

        for (int i = 0; i < OPT_ITEMS; i++)
        {
            Color c;
            if (i == state.menuSelection && state.editing)
                c = GREEN;
            else if (i == state.menuSelection)
                c = YELLOW;
            else
                c = WHITE;

            DrawText(items[i], x, y, fs, c);
            x += widths[i] + spacing;
        }

        const char* hint;
        if (state.editing)
            hint = "Left/Right: change    Space/Enter: stop editing";
        else
            hint = "Left/Right: navigate    Space/Enter: edit / back";

        float hw = MeasureText(hint, 18);
        DrawText(hint, screenWidth / 2.0f - hw / 2.0f, screenHeight * 0.75f, 18, GRAY);

        return;
    }

    ClearBackground(state.bgColor);

    float colW = (float)screenWidth / (float)NUM_COLORS;

    for (int i = 0; i < NUM_COLORS; i++)
    {
        Rectangle sq = { i * colW, state.wallY, colW, colW };
        DrawRectangleRec(sq, GetRainbowColor(i));
    }

    float hopOffset = 0.0f;
    if (state.hopTimer > 0.0f)
    {
        float t = state.hopTimer / 0.12f;
        hopOffset = -6.0f * 4.0f * t * (1.0f - t);
    }

    Vector2 a = { state.player.a.x, state.player.a.y + hopOffset };
    Vector2 b = { state.player.b.x, state.player.b.y + hopOffset };
    Vector2 c = { state.player.c.x, state.player.c.y + hopOffset };

    DrawTriangle(a, b, c, BLACK);

    DrawText(TextFormat("Score: %d", state.score), 10, 10, 28, WHITE);

    float lw = MeasureText("Lives: 99", 28);
    DrawText(TextFormat("Lives: %d", state.lives), screenWidth - lw - 10, 10, 28, WHITE);

    if (state.phase == PHASE_GAMEOVER)
    {
        DrawRectangle(0, 0, screenWidth, screenHeight, { 0, 0, 0, 180 });
        const char* msg = "GAME OVER";
        float mw = MeasureText(msg, 60);
        DrawText(msg, screenWidth / 2.0f - mw / 2.0f, screenHeight / 2.0f - 60, 60, WHITE);
        const char* restart = "Press SPACE or ENTER to restart";
        float rw = MeasureText(restart, 24);
        DrawText(restart, screenWidth / 2.0f - rw / 2.0f, screenHeight / 2.0f + 20, 24, WHITE);
    }
}
