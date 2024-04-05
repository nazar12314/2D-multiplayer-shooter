#include "SpriteRenderer.h"

#include "Material.h"

SpriteRenderer::SpriteRenderer(Object* obj, const Sprite* texture, glm::vec2 size, const Color& color, int order): BaseRenderer(obj, size, color, order)
{
	_material = new Material(texture, color);
}
