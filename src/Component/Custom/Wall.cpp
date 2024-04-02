#include "Wall.h"

#include "Rigidbody.h"
#include "SpriteRenderer.h"
#include "Assets.h"

Wall::Wall(Object* obj, glm::vec2 size): Component(obj)
{
	obj->setTag("Wall");

	auto tex = Assets::load<Texture>("sprites/square.png");
	obj->addComponent<SpriteRenderer>(tex, size);
	obj->addComponent<PolygonCollider>(size);
	obj->addComponent<Rigidbody>();
}
