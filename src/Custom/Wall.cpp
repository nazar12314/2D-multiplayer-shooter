#include "Wall.h"

#include "Rigidbody.h"
#include "SpriteRenderer.h"
#include "Assets.h"

Wall::Wall(Object* obj, glm::vec2 size): Component(obj)
{
	obj->setTag("Wall");

	auto tex = Assets::load<Texture>("sprites/square.png");
	addComponent<SpriteRenderer>(tex, size);
	addComponent<PolygonCollider>(size);
	addComponent<Rigidbody>();
}
