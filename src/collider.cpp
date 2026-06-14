#include <cmath>
#include "collider.h"

Vector2 TriangleCenter(Triangle t)
{
    return {
        (t.a.x + t.b.x + t.c.x) / 3.0f,
        (t.a.y + t.b.y + t.c.y) / 3.0f
    };
}

Rectangle TriangleAABB(Triangle t)
{
    float minX = fmin(fmin(t.a.x, t.b.x), t.c.x);
    float maxX = fmax(fmax(t.a.x, t.b.x), t.c.x);
    float minY = fmin(fmin(t.a.y, t.b.y), t.c.y);
    float maxY = fmax(fmax(t.a.y, t.b.y), t.c.y);
    return { minX, minY, maxX - minX, maxY - minY };
}

static float Sign(Vector2 p1, Vector2 p2, Vector2 p3)
{
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool PointInTriangle(Vector2 p, Triangle t)
{
    float d1 = Sign(p, t.a, t.b);
    float d2 = Sign(p, t.b, t.c);
    float d3 = Sign(p, t.c, t.a);

    bool hasNeg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    bool hasPos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(hasNeg && hasPos);
}

static Vector2 Sub(Vector2 a, Vector2 b)
{
    return { a.x - b.x, a.y - b.y };
}

static float Cross2D(Vector2 a, Vector2 b)
{
    return a.x * b.y - a.y * b.x;
}

static bool SegmentsIntersect(Vector2 p1, Vector2 q1, Vector2 p2, Vector2 q2)
{
    Vector2 r = Sub(q1, p1);
    Vector2 s = Sub(q2, p2);
    float rxs = Cross2D(r, s);

    if (fabsf(rxs) < 1e-8f)
        return false;

    Vector2 diff = Sub(p2, p1);
    float t = Cross2D(diff, s) / rxs;
    float u = Cross2D(diff, r) / rxs;

    return t >= 0.0f && t <= 1.0f && u >= 0.0f && u <= 1.0f;
}

bool TriangleOverlap(Triangle t1, Triangle t2)
{
    Vector2 edges1[] = { t1.a, t1.b, t1.c };
    Vector2 edges2[] = { t2.a, t2.b, t2.c };

    for (int i = 0; i < 3; i++)
    {
        Vector2 p1 = edges1[i];
        Vector2 q1 = edges1[(i + 1) % 3];

        for (int j = 0; j < 3; j++)
        {
            Vector2 p2 = edges2[j];
            Vector2 q2 = edges2[(j + 1) % 3];

            if (SegmentsIntersect(p1, q1, p2, q2))
                return true;
        }
    }

    return PointInTriangle(t1.a, t2)
        || PointInTriangle(t2.a, t1);
}

bool TriangleCircleOverlap(Triangle t, Vector2 center, float radius)
{
    if (PointInTriangle(center, t))
        return true;

    Vector2 verts[] = { t.a, t.b, t.c };
    for (int i = 0; i < 3; i++)
    {
        Vector2 p = verts[i];
        Vector2 q = verts[(i + 1) % 3];

        float dx = q.x - p.x;
        float dy = q.y - p.y;
        float lenSq = dx * dx + dy * dy;

        if (lenSq == 0)
            continue;

        float tParam = fmax(0.0f, fmin(1.0f,
            ((center.x - p.x) * dx + (center.y - p.y) * dy) / lenSq));

        Vector2 closest = { p.x + tParam * dx, p.y + tParam * dy };
        float distSq = (center.x - closest.x) * (center.x - closest.x)
                     + (center.y - closest.y) * (center.y - closest.y);

        if (distSq <= radius * radius)
            return true;
    }

    return false;
}
