#include "SpriteRenderer.h"

#include "Texture.h"

SpriteRenderer::SpriteRenderer(GameObject* obj, const Sprite* sprite, glm::vec2 size, const Color& color, int order): RectRenderer(obj, size, color, order)
{
	_texture = new Texture(sprite, color);
}
void SpriteRenderer::setSprite(const Sprite* sprite)
{
	delete _texture;
	_texture = new Texture(sprite, _color);
}
