#include "Solver.h"

#include "Collider.h"
#include "MyMath.h"
#include "Rigidbody.h"
#include "glm/geometric.hpp"
#include "glm/gtx/string_cast.hpp"

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
	for (auto& collision : collisions)
	{
		auto rb1 = collision.col1->_rb;
		auto rb2 = collision.col2->_rb;
		if (rb1->isStatic() && rb2->isStatic()) continue;

		auto e = std::min(rb1->_restitution, rb2->_restitution);
		auto contactPoint = collision.contactPoints.size() == 1 ? collision.contactPoints[0] : (collision.contactPoints[0] + collision.contactPoints[1]) / 2.0f;

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
		auto impulse = j * collision.norm;

		rb1->_velocity -= impulse * rb1->_invMass;
		rb1->_angularVelocity -= Math::cross(r1, impulse) * rb1->_invInertia;
		rb2->_velocity += impulse * rb2->_invMass;
		rb2->_angularVelocity += Math::cross(r2, impulse) * rb2->_invInertia;



		// Friction
		auto sf = (rb1->_staticFriction + rb2->_staticFriction) / 2;
		auto df = (rb1->_dynamicFriction + rb2->_dynamicFriction) / 2;

		angularVel1 = r1Perp * rb1->_angularVelocity;
		angularVel2 = r2Perp * rb2->_angularVelocity;

		relVel = (rb2->_velocity + angularVel2) - (rb1->_velocity + angularVel1);

		auto tangent = relVel - collision.norm * dot(relVel, collision.norm);
		if (Math::nearlyZero(tangent)) continue;
		tangent = normalize(tangent);

		auto r1PerpDotT = dot(r1Perp, tangent);
		auto r2PerpDotT = dot(r2Perp, tangent);

		denom = rb1->_invMass + rb2->_invMass +
			r1PerpDotT * r1PerpDotT * rb1->_invInertia +
			r2PerpDotT * r2PerpDotT * rb2->_invInertia;

		auto jt = -dot(relVel, tangent);
		jt /= denom;

		glm::vec2 frictionImpulse;
		if (std::abs(jt) <= j * sf)
			frictionImpulse = jt * tangent;
		else
			frictionImpulse = -j * tangent * df;

		rb1->_velocity -= frictionImpulse * rb1->_invMass;
		rb1->_angularVelocity -= Math::cross(r1, frictionImpulse) * rb1->_invInertia;
		rb2->_velocity += frictionImpulse * rb2->_invMass;
		rb2->_angularVelocity += Math::cross(r2, frictionImpulse) * rb2->_invInertia;
	}
}
