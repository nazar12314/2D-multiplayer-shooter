#include "Math.h"

#include <cmath>
#include <bits/std_abs.h>
#include <bits/stl_algo.h>


float MyMath::shortestAngle(float currentAngle, float targetAngle)
{
	float angle = fmod(targetAngle - currentAngle, 360.0f);
	if (angle < -180) angle += 360;
	else if (angle > 180) angle -= 360;
	return angle;
}
float MyMath::rotateTowards(float currentAngle, float targetAngle, float maxStep)
{
	float angleDiff = shortestAngle(currentAngle, targetAngle);
	float step = std::copysign(std::min(std::abs(angleDiff), maxStep), angleDiff);
	return currentAngle + step;
}
