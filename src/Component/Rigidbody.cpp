#include "Collider.h"
#include "Math.h"
#include "RigidBody.h"
#include "glm/gtx/string_cast.hpp"

Rigidbody::Rigidbody(Object* obj, float mass, float linearDrag, float angularDrag): Component(obj),
                                                                                    _mass(mass), _invMass(1 / mass), _linearDrag(linearDrag), _angularDrag(angularDrag)
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

	_inertia = _attachedCollider->calculateInertia(_mass);
	_invInertia = 1 / _inertia;
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
	_velocity += _force * _invMass * dt;
	if (!Math::nearlyEqual(_velocity.x, 0) || !Math::nearlyEqual(_velocity.y, 0))
	{
		obj->setPos(obj->pos() + _velocity * dt);
		_velocity *= 1 - _linearDrag * dt;
	}
	_force = {0, 0};

	_angularVelocity += _angularForce * _invMass * dt;
	if (!Math::nearlyEqual(_angularVelocity, 0))
	{
		obj->setRot(obj->rot() - glm::degrees(_angularVelocity) * dt);
		_angularVelocity *= 1 - _angularDrag * dt;
	}
	_angularForce = 0;
}

void Rigidbody::setIsStatic(bool isStatic) { _isStatic = isStatic; }
void Rigidbody::setMass(float mass) { _mass = mass; }
void Rigidbody::setRestitution(float restitution) { _restitution = restitution; }
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
