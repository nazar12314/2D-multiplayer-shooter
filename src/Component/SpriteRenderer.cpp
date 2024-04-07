#include "SpriteRenderer.h"

#include "Material.h"

SpriteRenderer::SpriteRenderer(GameObject* obj, const Sprite* sprite, glm::vec2 size, const Color& color, int order): BaseRenderer(obj, size, color, order)
{
	_material = new Material(sprite, color);
}
