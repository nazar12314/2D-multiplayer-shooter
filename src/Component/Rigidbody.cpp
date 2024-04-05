#include "Collider.h"
#include "MyMath.h"
#include "RigidBody.h"
#include "Transform.h"
#include "glm/trigonometric.hpp"

Rigidbody::Rigidbody(Object* obj, bool isStatic) : Component(obj), _isStatic(isStatic)
{
	initCollider();
}
Rigidbody::Rigidbody(Object* obj, float linearDrag, float angularDrag): Component(obj), _isStatic(false), _linearDrag(linearDrag), _angularDrag(angularDrag)
{
	initCollider();
}
void Rigidbody::initCollider()
{
	Collider* col;
	if (tryGetComponent<Collider>(col))
		attachCollider(col);
}

Rigidbody::~Rigidbody()
{
	resetCollider();
}

void Rigidbody::attachCollider(Collider* col)
{
	_attachedCollider = col;
	col->_rb = this;

	_mass = _isStatic ? std::numeric_limits<float>::max() : col->calculateMass();
	_invMass = _isStatic ? 0 : 1 / _mass;
	_inertia = _isStatic ? std::numeric_limits<float>::max() : _attachedCollider->calculateInertia(_mass);
	_invInertia = _isStatic ? 0 : 1 / _inertia;
}
void Rigidbody::resetCollider()
{
	if (_attachedCollider != nullptr)
	{
		_attachedCollider->_rb = nullptr;
		_attachedCollider = nullptr;
	}
}

void Rigidbody::step(float dt)
{
	if (_isStatic) return;

	if (!Math::nearlyZero(_velocity))
		_velocity *= 1 - _linearDrag * dt;

	if (!Math::nearlyZero(_angularVelocity))
		_angularVelocity *= 1 - _angularDrag * dt;
}
void Rigidbody::substep(float dt)
{
	if (_isStatic) return;
	if (_attachedCollider && transform()->hasChanged)
	{
		_attachedCollider->recalculate();
		transform()->hasChanged = false;
	}

	// Apply gravity
	_force += glm::vec2(0, -_gravity * _mass);

	_velocity += _force * _invMass * dt;
	if (!Math::nearlyZero(_velocity))
	{
		auto newPos = transform()->getPos() + _velocity * dt;
		transform()->setPos(newPos);
	}
	_force = {0, 0};

	_angularVelocity += _angularForce * _invMass * dt;
	if (!Math::nearlyZero(_angularVelocity))
	{
		float currRot = transform()->getRot();
		float newRot = currRot + glm::degrees(_angularVelocity) * dt;
		transform()->setRot(newRot);
	}
	_angularForce = 0;
}

void Rigidbody::setIsStatic(bool isStatic) { _isStatic = isStatic; }
void Rigidbody::setMass(float mass) { _mass = mass; }
void Rigidbody::setRestitution(float restitution) { _restitution = restitution; }
void Rigidbody::setStaticFriction(float staticFriction) { _staticFriction = staticFriction; }
void Rigidbody::setDynamicFriction(float dynamicFriction) { _dynamicFriction = dynamicFriction; }
void Rigidbody::setGravity(float gravity) { _gravity = gravity; }
void Rigidbody::setLinearDrag(float linearDrag) { _linearDrag = linearDrag; }
void Rigidbody::setAngularDrag(float angularDrag) { _angularDrag = angularDrag; }
void Rigidbody::setVelocity(glm::vec2 velocity) { _velocity = velocity; }
void Rigidbody::setAngularVelocity(float angularVelocity) { _angularVelocity = angularVelocity; }

void Rigidbody::addForce(glm::vec2 force)
{
	_force += force;
}
void Rigidbody::addAngularForce(float force)
{
	_angularForce += force;
}

void Rigidbody::moveTo(glm::vec2 pos) const
{
	transform()->setPos(pos);
	if (_attachedCollider != nullptr)
		_attachedCollider->recalculate();
}
void Rigidbody::rotateTo(float rot) const
{
	transform()->setRot(rot);
	if (_attachedCollider != nullptr)
		_attachedCollider->recalculate();
}

void Rigidbody::applyImpulse(glm::vec2 pos, glm::vec2 impulse)
{
	if (_isStatic) return;

	_velocity += impulse * _invMass;
	auto r = pos - transform()->getPos();
	_angularVelocity += Math::cross(r, impulse) * _invInertia;
}
void Rigidbody::applyImpact(glm::vec2 point, float radius, float force)
{
	if (_isStatic) return;

	auto collision = _attachedCollider->getImpactCollision(point, radius);
	if (collision == std::nullopt || collision->depth <= 0) return;

	auto contactPoint = collision->contactPoints[0];
	auto distanceFactor = 1 - std::pow(length(point - contactPoint) / radius, 1.5f);
	auto impulse = normalize(contactPoint - point) * distanceFactor * force;
	applyImpulse(point, impulse);
}
