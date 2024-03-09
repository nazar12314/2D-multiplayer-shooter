#include "Graphical.h"

#include <SDL_render.h>

#include "Renderer.h"
#include "Texture.h"

Graphical::Graphical(Texture* texture, glm::ivec2 size, const glm::vec2& pos, float rot) : Object(pos, rot), texture(texture), size{ size }
{
	graphicalObjects.emplace_back(this);
}
Graphical::~Graphical()
{
	graphicalObjects.erase(std::ranges::find(graphicalObjects, this));
}

void Graphical::draw(const glm::vec2& cameraPos, int cameraSize) const
{
	auto relPos = (pos - cameraPos) / (float)cameraSize;
	auto relSize = (glm::vec2)size / (float)cameraSize;
	Renderer::renderTexNormalized(texture, relPos, relSize);
}

void Graphical::setNativeSize()
{
	SDL_QueryTexture(texture->texture, nullptr, nullptr, &this->size.x, &this->size.y);
}
