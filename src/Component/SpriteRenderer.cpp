#include "SpriteRenderer.h"

#include <SDL_render.h>

#include "Object.h"
#include "Renderer.h"
#include "Assets.h"
#include "Material.h"

SpriteRenderer::SpriteRenderer(Object* obj, Texture* texture, glm::vec2 size, int order, const Color& color): Component(obj), _size(size), _order(order)
{
	_material = new Material(texture);
	if (color != Color::white)
		setColor(color);
}
SpriteRenderer::~SpriteRenderer()
{
	delete _material;
}

Material* SpriteRenderer::material() const { return _material; }
glm::vec2 SpriteRenderer::size() const { return this->_size; }
int SpriteRenderer::order() const { return _order; }

void SpriteRenderer::setNativeSize()
{
	glm::ivec2 size;
	SDL_QueryTexture(_material->texture(), nullptr, nullptr, &size.x, &size.y);
	this->_size = size;
}
void SpriteRenderer::setOrder(int order)
{
	this->_order = order;
	Renderer::sortSprites();
}
void SpriteRenderer::setColor(const Color& color) const
{
	_material->setColor(color);
}
