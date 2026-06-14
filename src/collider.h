#pragma once
#include "raylib.h"

struct Triangle
{
    Vector2 a;
    Vector2 b;
    Vector2 c;
};

Vector2    TriangleCenter(Triangle t);
Rectangle  TriangleAABB(Triangle t);
bool       PointInTriangle(Vector2 p, Triangle t);
bool       TriangleOverlap(Triangle t1, Triangle t2);
bool       TriangleCircleOverlap(Triangle t, Vector2 center, float radius);
