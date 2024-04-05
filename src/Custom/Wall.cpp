#include "Wall.h"

#include "Rigidbody.h"
#include "SpriteRenderer.h"
#include "Assets.h"

Wall::Wall(Object* obj, glm::vec2 size): Component(obj)
{
	obj->setTag("Wall");

	addComponent<SpriteRenderer>(Assets::load<Sprite>("assets/sprites/square.png"), size);
	addComponent<PolygonCollider>(size);
	auto rb = addComponent<Rigidbody>(true);
	rb->setRestitution(0.5f);
}
