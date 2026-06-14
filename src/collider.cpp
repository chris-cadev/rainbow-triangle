#include <cmath>
#include "collider.h"

Vector2 TriangleCenter(Triangle triangle)
{
    return {
        (triangle.a.x + triangle.b.x + triangle.c.x) / 3.0f,
        (triangle.a.y + triangle.b.y + triangle.c.y) / 3.0f
    };
}

Rectangle TriangleAABB(Triangle triangle)
{
    float minimumX = fmin(fmin(triangle.a.x, triangle.b.x), triangle.c.x);
    float maximumX = fmax(fmax(triangle.a.x, triangle.b.x), triangle.c.x);
    float minimumY = fmin(fmin(triangle.a.y, triangle.b.y), triangle.c.y);
    float maximumY = fmax(fmax(triangle.a.y, triangle.b.y), triangle.c.y);
    return { minimumX, minimumY, maximumX - minimumX, maximumY - minimumY };
}

static float Sign(Vector2 pointA, Vector2 pointB, Vector2 pointC)
{
    return (pointA.x - pointC.x) * (pointB.y - pointC.y) - (pointB.x - pointC.x) * (pointA.y - pointC.y);
}

bool PointInTriangle(Vector2 point, Triangle triangle)
{
    float signAB = Sign(point, triangle.a, triangle.b);
    float signBC = Sign(point, triangle.b, triangle.c);
    float signCA = Sign(point, triangle.c, triangle.a);

    bool hasNegative = (signAB < 0) || (signBC < 0) || (signCA < 0);
    bool hasPositive = (signAB > 0) || (signBC > 0) || (signCA > 0);

    return !(hasNegative && hasPositive);
}

static bool SegmentsIntersect(Vector2 segmentAStart, Vector2 segmentAEnd, Vector2 segmentBStart, Vector2 segmentBEnd)
{
    float edgeAX = segmentAEnd.x - segmentAStart.x;
    float edgeAY = segmentAEnd.y - segmentAStart.y;
    float edgeBX = segmentBEnd.x - segmentBStart.x;
    float edgeBY = segmentBEnd.y - segmentBStart.y;
    float crossProduct = edgeAX * edgeBY - edgeAY * edgeBX;

    if (fabsf(crossProduct) < 1e-8f)
        return false;

    float startDeltaX = segmentBStart.x - segmentAStart.x;
    float startDeltaY = segmentBStart.y - segmentAStart.y;
    float projectionA = (startDeltaX * edgeBY - startDeltaY * edgeBX) / crossProduct;
    float projectionB = (startDeltaX * edgeAY - startDeltaY * edgeAX) / crossProduct;

    return projectionA >= 0.0f && projectionA <= 1.0f && projectionB >= 0.0f && projectionB <= 1.0f;
}

bool TriangleOverlap(Triangle triangleA, Triangle triangleB)
{
    Vector2 edgesA[] = { triangleA.a, triangleA.b, triangleA.c };
    Vector2 edgesB[] = { triangleB.a, triangleB.b, triangleB.c };

    for (int i = 0; i < 3; i++)
    {
        Vector2 segmentAStart = edgesA[i];
        Vector2 segmentAEnd = edgesA[(i + 1) % 3];

        for (int j = 0; j < 3; j++)
        {
            Vector2 segmentBStart = edgesB[j];
            Vector2 segmentBEnd = edgesB[(j + 1) % 3];

            if (SegmentsIntersect(segmentAStart, segmentAEnd, segmentBStart, segmentBEnd))
                return true;
        }
    }

    return PointInTriangle(triangleA.a, triangleB)
        || PointInTriangle(triangleB.a, triangleA);
}

bool TriangleCircleOverlap(Triangle triangle, Vector2 center, float radius)
{
    if (PointInTriangle(center, triangle))
        return true;

    Vector2 vertices[] = { triangle.a, triangle.b, triangle.c };
    for (int i = 0; i < 3; i++)
    {
        Vector2 edgeStart = vertices[i];
        Vector2 edgeEnd = vertices[(i + 1) % 3];

        float edgeX = edgeEnd.x - edgeStart.x;
        float edgeY = edgeEnd.y - edgeStart.y;
        float lengthSquared = edgeX * edgeX + edgeY * edgeY;

        if (lengthSquared == 0)
            continue;

        float projectionFactor = fmax(0.0f, fmin(1.0f,
            ((center.x - edgeStart.x) * edgeX + (center.y - edgeStart.y) * edgeY) / lengthSquared));

        Vector2 closestPoint = { edgeStart.x + projectionFactor * edgeX, edgeStart.y + projectionFactor * edgeY };
        float distanceSquared = (center.x - closestPoint.x) * (center.x - closestPoint.x)
                              + (center.y - closestPoint.y) * (center.y - closestPoint.y);

        if (distanceSquared <= radius * radius)
            return true;
    }

    return false;
}
