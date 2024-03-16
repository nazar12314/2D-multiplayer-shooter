#include "Sprite.h"

#include <SDL_render.h>

#include "Object.h"
#include "Renderer.h"
#include "Texture.h"

Sprite::Sprite(Object* obj, Texture* texture, glm::vec2 size): Component(obj), _texture(texture), _size(size) {}

Texture* Sprite::texture() const { return _texture; }
glm::vec2 Sprite::size() const { return this->_size; }

void Sprite::setNativeSize()
{
	glm::ivec2 size;
	SDL_QueryTexture(_texture->texture, nullptr, nullptr, &size.x, &size.y);
	this->_size = size;
}
