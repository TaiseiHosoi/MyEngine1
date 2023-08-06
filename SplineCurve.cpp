#include "SplineCurve.h"

SplineCurve::SplineCurve(const std::vector<Vector3>& controlPoints)
    : controlPoints(controlPoints)
{
    numControlPoints = static_cast<int>(controlPoints.size());
    currentProgress = 0.0f;
    currentPointIndex = 0;
}

Vector3 SplineCurve::getCurrentPosition() const {
    int startIndex = currentPointIndex;
    int endIndex = (currentPointIndex + 1) % numControlPoints;

    float t = currentProgress;
    Vector3 startPoint = controlPoints[startIndex];
    Vector3 endPoint = controlPoints[endIndex];

    float t2 = t * t;
    float t3 = t2 * t;
    float h1 = 2.0f * t3 - 3.0f * t2 + 1.0f;
    float h2 = -2.0f * t3 + 3.0f * t2;
    float h3 = t3 - 2.0f * t2 + t;
    float h4 = t3 - t2;

    Vector3 position;
    position.x = h1 * startPoint.x + h2 * endPoint.x + h3 * (controlPoints[(startIndex + numControlPoints - 1) % numControlPoints].x) + h4 * (controlPoints[(endIndex + 1) % numControlPoints].x);
    position.y = h1 * startPoint.y + h2 * endPoint.y + h3 * (controlPoints[(startIndex + numControlPoints - 1) % numControlPoints].y) + h4 * (controlPoints[(endIndex + 1) % numControlPoints].y);
    position.z = h1 * startPoint.z + h2 * endPoint.z + h3 * (controlPoints[(startIndex + numControlPoints - 1) % numControlPoints].z) + h4 * (controlPoints[(endIndex + 1) % numControlPoints].z);

    return position;
}

void SplineCurve::update(float deltaTime) {
    currentProgress += deltaTime;
    if (currentProgress >= 1.0f) {
        currentProgress = 0.0f; //リセット
        currentPointIndex = (currentPointIndex + 1) % numControlPoints;
    }
}

float SplineCurve::getCurrentProgress() const {
    return currentProgress;
}

int SplineCurve::getCurrentPointIndex() const {
    return currentPointIndex;
}