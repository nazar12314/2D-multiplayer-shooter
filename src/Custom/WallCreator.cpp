#include "WallCreator.h"

#include "Assets.h"
#include "Object.h"
#include "PolygonCollider.h"
#include "SpriteRenderer.h"
#include "Rigidbody.h"

void WallCreator::createWall(glm::vec2 position, float rotation, glm::vec2 size)
{
	auto wall = Object::create("Wall", position, rotation);
	wall->setTag("Wall");
	wall->addComponent<SpriteRenderer>(Assets::load<Texture>("sprites/square.png"), size);
	wall->addComponent<PolygonCollider>(size);
	auto rb = wall->addComponent<Rigidbody>();
	rb->setRestitution(0.5f);
}
