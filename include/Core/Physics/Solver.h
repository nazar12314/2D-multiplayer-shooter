#pragma once

#include <vector>

#include "Collision.h"

class Rigidbody;

class Solver
{
public:
	virtual void solveCollisions(const std::vector<Collision>& collisions) = 0;
};


class PositionSolver : public Solver
{
public:
	void solveCollisions(const std::vector<Collision>& collisions) override;
};

class ImpulseSolver : public Solver
{
public:
	void solveCollisions(const std::vector<Collision>& collisions) override;
};
