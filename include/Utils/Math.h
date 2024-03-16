#pragma once

class MyMath
{
	static float shortestAngle(float currentAngle, float targetAngle);

public:
	static float rotateTowards(float currentAngle, float targetAngle, float maxStep);
};
