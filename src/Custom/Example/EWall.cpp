#include "EWall.h"

#include "Rigidbody.h"
#include "SpriteRenderer.h"
#include "Assets.h"

namespace examples
{
	EWall::EWall(GameObject* obj, glm::vec2 size): Component(obj)
	{
		obj->setTag("Wall");

		addComponent<SpriteRenderer>(Assets::load<Sprite>("assets/sprites/square.png"), size);
		addComponent<BoxCollider>(size);
		auto rb = addComponent<Rigidbody>(true);
		rb->setRestitution(0.5f);
	}
}
