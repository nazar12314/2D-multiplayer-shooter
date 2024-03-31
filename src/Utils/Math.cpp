#include "Math.h"

#include <cmath>
#include <bits/std_abs.h>
#include <bits/stl_algo.h>


float Math::shortestAngle(float currentAngle, float targetAngle)
{
	float angle = fmod(targetAngle - currentAngle, 360.0f);
	if (angle < -180) angle += 360;
	else if (angle > 180) angle -= 360;
	return angle;
}

float Math::rotateTowards(float currentAngle, float targetAngle, float maxStep)
{
	float angleDiff = shortestAngle(currentAngle, targetAngle);
	float step = std::copysign(std::min(std::abs(angleDiff), maxStep), angleDiff);
	return currentAngle + step;
}

float Math::lerp(float a, float b, float t)
{
	return a + (b - a) * t;
}

double Math::distance(double startX, double startY, double endX, double endY)
{
	double a = std::abs(startX - endX);
	double b = std::abs(startY - endY);

	return std::sqrt(a * a + b * b);
}
