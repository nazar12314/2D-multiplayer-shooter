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
		resolutionVector /= rb1->_invMass + rb2->_invMass;
		rb1->moveTo(rb1->obj->pos() - resolutionVector * rb1->_invMass);
		rb2->moveTo(rb2->obj->pos() + resolutionVector * rb2->_invMass);
	}
}

void ImpulseSolver::solveCollisions(const std::vector<Collision>& collisions)
{
	static std::vector<glm::vec2> r1Values(2);
	static std::vector<glm::vec2> r2Values(2);
	static std::vector<glm::vec2> impulses(2);
	static std::vector<glm::vec2> frictionImpulses(2);
	static std::vector<float> jValues(2);

	for (auto& collision : collisions)
	{
		auto rb1 = collision.col1->_rb;
		auto rb2 = collision.col2->_rb;
		if (rb1->isStatic() && rb2->isStatic()) continue;

		auto e = std::min(rb1->_restitution, rb2->_restitution);

		r1Values.clear();
		r2Values.clear();
		impulses.clear();
		jValues.clear();
		for (int i = 0; i < collision.contactPoints.size(); i++)
		{
			auto contactPoint = collision.contactPoints[i];

			auto r1 = contactPoint - rb1->obj->pos();
			auto r2 = contactPoint - rb2->obj->pos();
			r1Values.push_back(r1);
			r2Values.push_back(r2);

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
			jValues.push_back(j);

			auto impulse = j * collision.norm;
			impulses.push_back(impulse);
		}

		for (int i = 0; i < impulses.size(); i++)
		{
			auto impulse = impulses[i];

			rb1->_velocity -= impulse * rb1->_invMass;
			rb1->_angularVelocity -= Math::cross(r1Values[i], impulse) * rb1->_invInertia;
			rb2->_velocity += impulse * rb2->_invMass;
			rb2->_angularVelocity += Math::cross(r2Values[i], impulse) * rb2->_invInertia;
		}

		auto sf = (rb1->_staticFriction + rb2->_staticFriction) / 2;
		auto df = (rb1->_dynamicFriction + rb2->_dynamicFriction) / 2;

		// Friction
		frictionImpulses.clear();
		for (int i = 0; i < jValues.size(); i++)
		{
			auto r1 = r1Values[i];
			auto r2 = r2Values[i];

			auto r1Perp = glm::vec2(-r1.y, r1.x);
			auto r2Perp = glm::vec2(-r2.y, r2.x);

			auto angularVel1 = r1Perp * rb1->_angularVelocity;
			auto angularVel2 = r2Perp * rb2->_angularVelocity;

			auto relVel = (rb2->_velocity + angularVel2) - (rb1->_velocity + angularVel1);

			auto tangent = relVel - collision.norm * dot(relVel, collision.norm);
			if (Math::nearlyZero(tangent)) continue;
			tangent = normalize(tangent);

			auto r1PerpDotT = dot(r1Perp, tangent);
			auto r2PerpDotT = dot(r2Perp, tangent);

			auto denom = rb1->_invMass + rb2->_invMass +
				r1PerpDotT * r1PerpDotT * rb1->_invInertia +
				r2PerpDotT * r2PerpDotT * rb2->_invInertia;

			auto jt = -dot(relVel, tangent);
			jt /= denom;
			jt /= collision.contactPoints.size();

			auto j = jValues[i];

			glm::vec2 frictionImpulse;
			if (std::abs(jt) <= j * sf) 
				frictionImpulse = jt * tangent;
			else
				frictionImpulse = -j * tangent * df;

			frictionImpulses.push_back(frictionImpulse);
		}

		for (int i = 0; i < frictionImpulses.size(); i++)
		{
			auto frictionImpulse = frictionImpulses[i];

			rb1->_velocity -= frictionImpulse * rb1->_invMass;
			rb1->_angularVelocity -= Math::cross(r1Values[i], frictionImpulse) * rb1->_invInertia;
			rb2->_velocity += frictionImpulse * rb2->_invMass;
			rb2->_angularVelocity += Math::cross(r2Values[i], frictionImpulse) * rb2->_invInertia;
		}

	}
}
