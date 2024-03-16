#include "RigidBody.h"

RigidBody::RigidBody(Object* obj, float linearDrag, float angularDrag, float mass): Component(obj), linearDrag(linearDrag), angularDrag(angularDrag), mass(mass) {}

void RigidBody::setVelocity(glm::vec2 velocity)
{
	_velocity = velocity;
}
void RigidBody::addVelocity(glm::vec2 velocity)
{
	_velocity += velocity;
}
void RigidBody::setAngularVelocity(float angularVelocity)
{
	_angularVelocity = angularVelocity;
}
void RigidBody::addAngularVelocity(float angularVelocity)
{
	_angularVelocity += angularVelocity;
}

void RigidBody::moveTo(glm::vec2 pos) const
{
	// TODO: calculate new pos with physics
	obj->setPos(pos);
}
void RigidBody::rotateTo(float angle) const
{
	// TODO: calculate new angle with physics
	obj->setRot(angle);
}
