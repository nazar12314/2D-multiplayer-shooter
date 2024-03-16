#include "Sprite.h"

#include <SDL_render.h>

#include "Renderer.h"
#include "Texture.h"

Sprite::Sprite(Object* obj, Texture* texture, glm::ivec2 size): Component(obj), texture(texture), size(size) {}

void Sprite::draw(const glm::vec2& cameraPos, int cameraSize) const
{
	auto relPos = (obj->pos() - cameraPos) / (float)cameraSize;
	auto relSize = (glm::vec2)size / (float)cameraSize;
	Renderer::renderTexNormalized(texture, relPos, relSize, obj->rot());
}

void Sprite::setNativeSize()
{
	SDL_QueryTexture(texture->texture, nullptr, nullptr, &this->size.x, &this->size.y);
}
