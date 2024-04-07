#include "SpriteRenderer.h"

#include "Texture.h"

SpriteRenderer::SpriteRenderer(GameObject* obj, const Sprite* sprite, glm::vec2 size, const Color& color, int order): BaseRenderer(obj, size, color, order)
{
	_texture = new Texture(sprite, color);
}
