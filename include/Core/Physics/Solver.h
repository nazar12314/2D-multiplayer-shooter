#pragma once

#include <vector>

#include "Collision.h"

class PositionSolver
{
public:
	static void solveCollisions(const std::vector<Collision>& collisions);
};

class ImpulseSolver
{
public:
	static void solveCollisions(const std::vector<Collision>& collisions);
};
