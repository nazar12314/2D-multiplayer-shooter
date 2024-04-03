#include "Solver.h"

#include "Collider.h"
#include "MyMath.h"
#include "Rigidbody.h"
#include "glm/geometric.hpp"

void PositionSolver::solveCollisions(const std::vector<Collision>& collisions)
{
	for (auto& collision : collisions)
	{
		auto rb1 = collision.col1->_rb;
		auto rb2 = collision.col2->_rb;
		if (rb1->isStatic() && rb2->isStatic()) continue;

		auto resolutionVector = collision.norm * collision.depth;
		if (rb1->isStatic() || rb2->isStatic()) resolutionVector *= 2.0f;
		rb1->obj->setPos(rb1->obj->pos() - resolutionVector / 2.0f * (1.0f - rb1->isStatic()));
		rb2->obj->setPos(rb2->obj->pos() + resolutionVector / 2.0f * (1.0f - rb2->isStatic()));
	}
}

std::vector<glm::vec2> impulses(2);
void ImpulseSolver::solveCollisions(const std::vector<Collision>& collisions)
{
	for (auto& collision : collisions)
	{
		auto rb1 = collision.col1->_rb;
		auto rb2 = collision.col2->_rb;
		if (rb1->isStatic() && rb2->isStatic()) continue;

		auto e = std::min(rb1->_restitution, rb2->_restitution);

		impulses.clear();
		for (int i = 0; i < collision.contactPoints.size(); i++)
		{
			auto contactPoint = collision.contactPoints[i];

			auto r1 = contactPoint - rb1->obj->pos();
			auto r2 = contactPoint - rb2->obj->pos();

			auto r1Perp = glm::vec2(-r1.y, r1.x);
			auto r2Perp = glm::vec2(-r2.y, r2.x);

			auto angularVel1 = r1Perp * rb1->_angularVelocity;
			auto angularVel2 = r2Perp * rb2->_angularVelocity;

			auto relVel = (rb2->_velocity + angularVel2) - (rb1->_velocity + angularVel1);
			auto contactVelMag = dot(relVel, collision.norm);
			if (contactVelMag > 0) continue;

			auto r1PerpDotN = dot(r1Perp, collision.norm);
			auto r2PerpDotN = dot(r2Perp, collision.norm);

			auto denom = rb1->_invMass + rb2->_invMass +
				r1PerpDotN * r1PerpDotN * rb1->_invInertia +
				r2PerpDotN * r2PerpDotN * rb2->_invInertia;

			auto j = -(1.0f + e) * contactVelMag;
			j /= denom;
			j /= collision.contactPoints.size();

			auto impulse = j * collision.norm;
			impulses.push_back(impulse);
		}

		for (int i = 0; i < impulses.size(); i++)
		{
			auto impulse = impulses[i];
			auto contactPoint = collision.contactPoints[i];

			auto r1 = contactPoint - rb1->obj->pos();
			auto r2 = contactPoint - rb2->obj->pos();

			rb1->_velocity -= impulse * rb1->_invMass;
			rb1->_angularVelocity -= Math::cross(r1, impulse) * rb1->_invInertia;
			rb2->_velocity += impulse * rb2->_invMass;
			rb2->_angularVelocity += Math::cross(r2, impulse) * rb2->_invInertia;
		}
	}
}
