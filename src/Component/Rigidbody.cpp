#include "Collider.h"
#include "MyMath.h"
#include "RigidBody.h"
#include "glm/gtx/string_cast.hpp"

Rigidbody::Rigidbody(Object* obj, float linearDrag, float angularDrag): Component(obj), _isStatic(false), _linearDrag(linearDrag), _angularDrag(angularDrag)
{
	initCollider();
}
Rigidbody::Rigidbody(Object* obj) : Component(obj), _isStatic(true), _mass(std::numeric_limits<float>::max()), _invMass(0)
{
	initCollider();
}
void Rigidbody::initCollider()
{
	Collider* col;
	if (obj->tryGetComponent<Collider>(col))
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

	// Apply gravity
	_force += glm::vec2(0, -_gravity * _mass);

	bool moved = false;
	_velocity += _force * _invMass * dt;
	if (!Math::nearlyZero(_velocity))
	{
		obj->setPos(obj->pos() + _velocity * dt);
		//_velocity *= 1 - _linearDrag * dt;
		moved = true;
	}
	_force = {0, 0};

	_angularVelocity += _angularForce * _invMass * dt;
	if (!Math::nearlyZero(_angularVelocity))
	{
		obj->setRot(obj->rot() - glm::degrees(_angularVelocity) * dt);
		//_angularVelocity *= 1 - _angularDrag * dt;
		moved = true;
	}
	else _angularVelocity = 0;
	_angularForce = 0;

	if (moved && _attachedCollider != nullptr)
		_attachedCollider->recalculate();
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
	obj->setPos(pos);
	if (_attachedCollider != nullptr)
		_attachedCollider->recalculate();
}
void Rigidbody::rotateTo(float rot) const
{
	obj->setRot(rot);
	if (_attachedCollider != nullptr)
		_attachedCollider->recalculate();
}
