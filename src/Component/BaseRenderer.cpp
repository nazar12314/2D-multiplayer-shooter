#include "BaseRenderer.h"

#include "Camera.h"
#include "Renderer.h"
#include "SDLHandler.h"
#include "Texture.h"
#include "Transform.h"

BaseRenderer::BaseRenderer(GameObject* obj, glm::vec2 size, const Color& color, int order) : Component(obj), _size(size), _color(color), _order(order) {}
BaseRenderer::~BaseRenderer()
{
	delete _texture;
}
void BaseRenderer::render(const Camera* camera) const
{
	auto texture = _texture->texture();
	if (texture == nullptr) return;

	auto pos = transform()->getPos();
	auto screenPos = camera->worldToScreenPoint(pos);
	auto sizeOnScreen = getFinalSize() / (glm::vec2)camera->size() * (float)SDLHandler::windowSize.y;
	Renderer::renderTex(texture, screenPos, sizeOnScreen, transform()->getRot() - camera->transform()->getRot());
}

glm::vec2 BaseRenderer::getFinalSize() const
{
	if (!_preserveAspect) return _size * transform()->getScale();

	auto size = _size;
	auto ratio = _texture->getRatio();
	if (size.x / size.y > ratio)
		size.x = size.y * ratio;
	else
		size.y = size.x / ratio;
	return size * transform()->getScale();
}

Texture* BaseRenderer::texture() const { return _texture; }
int BaseRenderer::order() const { return _order; }
glm::vec2 BaseRenderer::size() const { return _size; }
Color BaseRenderer::color() const { return _color; }


void BaseRenderer::setSize(const glm::vec2& size) { this->_size = size; }
void BaseRenderer::setOrder(int order)
{
	this->_order = order;
	Renderer::sortRenderers();
}
void BaseRenderer::setColor(const Color& color)
{
	this->_color = color;
	_texture->setColor(color);
}
void BaseRenderer::setPreserveAspect(bool preserveAspect)
{
	_preserveAspect = preserveAspect;
}

void BaseRenderer::setNativeSize()
{
	glm::ivec2 size;
	SDL_QueryTexture(_texture->texture(), nullptr, nullptr, &size.x, &size.y);
	this->_size = size;
}
