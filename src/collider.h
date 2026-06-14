#pragma once
#include "raylib.h"

struct Triangle
{
    Vector2 a;
    Vector2 b;
    Vector2 c;
};

Vector2    TriangleCenter(Triangle triangle);
Rectangle  TriangleAABB(Triangle triangle);
bool       PointInTriangle(Vector2 point, Triangle triangle);
bool       TriangleOverlap(Triangle triangleA, Triangle triangleB);
bool       TriangleCircleOverlap(Triangle triangle, Vector2 center, float radius);
