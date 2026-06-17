#pragma once
#include "raylib.h"

constexpr Color colorRed        = {232, 11, 69, 255};
constexpr Color colorOrange     = {247, 114, 25, 255};
constexpr Color colorYellow     = {243, 220, 15, 255};
constexpr Color colorGreen      = {18, 231, 46, 255};
constexpr Color colorCyan       = {23, 235, 217, 255};
constexpr Color colorBlue       = {13, 92, 238, 255};
constexpr Color colorIndigo     = {138, 18, 218, 255};
constexpr Color backgroundColor = {40, 40, 50, 255};

inline Color MapToSafeColor(Color color)
{
    const Color colors[] = { colorRed, colorOrange, colorYellow, colorGreen, colorCyan, colorBlue, colorIndigo };
    const Color safeColors[] = {
        {188, 55, 95, 255},
        {195, 110, 60, 255},
        {195, 180, 60, 255},
        {70, 185, 85, 255},
        {65, 190, 175, 255},
        {60, 110, 190, 255},
        {140, 65, 175, 255}
    };
    for (int i = 0; i < 7; i++)
    {
        if (color.r == colors[i].r && color.g == colors[i].g && color.b == colors[i].b && color.a == colors[i].a)
            return safeColors[i];
    }
    return color;
}

inline Color GetRainbowColor(int index)
{
    const Color colors[] = { colorRed, colorOrange, colorYellow, colorGreen, colorCyan, colorBlue, colorIndigo };
    return colors[index % 7];
}

inline Color GetSafeRainbowColor(int index)
{
    const Color safeColors[] = {
        {188, 55, 95, 255},
        {195, 110, 60, 255},
        {195, 180, 60, 255},
        {70, 185, 85, 255},
        {65, 190, 175, 255},
        {60, 110, 190, 255},
        {140, 65, 175, 255}
    };
    return safeColors[index % 7];
}

