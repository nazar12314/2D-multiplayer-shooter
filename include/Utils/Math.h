#pragma once

class Math
{
	static float shortestAngle(float currentAngle, float targetAngle);

public:
	static float rotateTowards(float currentAngle, float targetAngle, float maxStep);
	static float lerp(float a, float b, float t);
	static double distance(double startX, double startY, double endX, double endY);
};
