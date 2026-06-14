#include "render.h"
#include "colors.h"
#include "state.h"

void DrawScene(const GameState& state, int screenWidth, int screenHeight)
{
    ClearBackground(state.bgColor);

    float colW = (float)screenWidth / (float)NUM_COLORS;

    // Rainbow wall: one square of each color in a row
    for (int i = 0; i < NUM_COLORS; i++)
    {
        Rectangle sq = { i * colW, state.wallY, colW, colW };
        DrawRectangleRec(sq, GetRainbowColor(i));
    }

    // Black upward-pointing triangle
    DrawTriangle(state.player.a, state.player.b, state.player.c, BLACK);

    // Score
    DrawText(TextFormat("Score: %d", state.score), 10, 10, 28, WHITE);

    // Lives
    float lw = MeasureText("Lives: 99", 28);
    DrawText(TextFormat("Lives: %d", state.lives), screenWidth - lw - 10, 10, 28, WHITE);

    if (state.phase == PHASE_GAMEOVER)
    {
        DrawRectangle(0, 0, screenWidth, screenHeight, { 0, 0, 0, 180 });
        const char* msg = "GAME OVER";
        float mw = MeasureText(msg, 60);
        DrawText(msg, screenWidth / 2.0f - mw / 2.0f, screenHeight / 2.0f - 60, 60, WHITE);
        const char* restart = "Press SPACE to restart";
        float rw = MeasureText(restart, 24);
        DrawText(restart, screenWidth / 2.0f - rw / 2.0f, screenHeight / 2.0f + 20, 24, WHITE);
    }
}
