#include "Sprite.h"

#include <SDL_render.h>

#include "Object.h"
#include "Renderer.h"
#include "Texture.h"

Sprite::Sprite(Object* obj, Texture* texture, glm::vec2 size, int order): Component(obj), _texture(texture), _size(size), _order(order) {}

Texture* Sprite::texture() const { return _texture; }
glm::vec2 Sprite::size() const { return this->_size; }
int Sprite::order() const { return _order; }

void Sprite::setNativeSize()
{
	glm::ivec2 size;
	SDL_QueryTexture(_texture->texture, nullptr, nullptr, &size.x, &size.y);
	this->_size = size;
}
void Sprite::setOrder(int order)
{
	this->_order = order;
	Renderer::sortSprites();
}
