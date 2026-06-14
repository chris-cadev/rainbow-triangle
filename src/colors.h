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

inline Color GetRainbowColor(int index)
{
    const Color colors[] = { colorRed, colorOrange, colorYellow, colorGreen, colorCyan, colorBlue, colorIndigo };
    return colors[index % 7];
}

inline const char* GetColorName(int index)
{
    const char* names[] = { "Red", "Orange", "Yellow", "Green", "Cyan", "Blue", "Indigo" };
    return names[index % 7];
}
