#include "Wall.h"

#include "Rigidbody.h"
#include "Sprite.h"
#include "Texture.h"

Wall::Wall(Object* obj, glm::vec2 size): Component(obj)
{
	obj->setTag("Wall");

	auto squareTex = new Texture("sprites/square.png");
	obj->addComponent<Sprite>(squareTex, size);
	obj->addComponent<PolygonCollider>(size);
	obj->addComponent<RigidBody>();
}
