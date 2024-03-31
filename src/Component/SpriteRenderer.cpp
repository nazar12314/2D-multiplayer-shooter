#include "SpriteRenderer.h"

#include <SDL_render.h>

#include "Object.h"
#include "Renderer.h"
#include "Assets.h"

SpriteRenderer::SpriteRenderer(Object* obj, Texture* texture, glm::vec2 size, int order, const Color& color): Component(obj), _texture(texture), _size(size), _order(order)
{
	if (color != Color::white)
		setColor(color);
}

Texture* SpriteRenderer::sprite() const { return _texture; }
glm::vec2 SpriteRenderer::size() const { return this->_size; }
int SpriteRenderer::order() const { return _order; }

void SpriteRenderer::setNativeSize()
{
	glm::ivec2 size;
	SDL_QueryTexture(_texture->texture(), nullptr, nullptr, &size.x, &size.y);
	this->_size = size;
}
void SpriteRenderer::setOrder(int order)
{
	this->_order = order;
	Renderer::sortSprites();
}
void SpriteRenderer::setColor(const Color& color)
{
	if (!clonedTexture)
	{
		_texture = new Texture(*_texture);
		clonedTexture = true;
	}

	_texture->setColor(color);
}

void SpriteRenderer::onDestroy()
{
	if (clonedTexture)
		delete _texture;
}
