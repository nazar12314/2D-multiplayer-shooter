#include "BaseRenderer.h"

#include "Camera.h"
#include "Renderer.h"
#include "SDLHandler.h"
#include "Texture.h"
#include "Transform.h"

BaseRenderer::BaseRenderer(GameObject* obj, const Color& color, int order) : Component(obj), _color(color), _order(order) {}
BaseRenderer::~BaseRenderer() { delete _texture; }

Texture* BaseRenderer::texture() const { return _texture; }
int BaseRenderer::order() const { return _order; }
Color BaseRenderer::color() const { return _color; }


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

RectRenderer::RectRenderer(GameObject* obj, glm::vec2 size, const Color& color, int order) : BaseRenderer(obj, color, order), _size(size) {}

void RectRenderer::render(const Camera* camera) const
{
	auto texture = _texture->texture();
	if (texture == nullptr) return;

	auto pos = transform()->pos();
	auto size = getFinalSize();
	float rot = transform()->rot();
	Renderer::renderTexWorld(texture, pos, size, rot, camera);
}
glm::vec2 RectRenderer::getFinalSize() const
{
	if (!_preserveAspect) return _size * transform()->scale();

	auto size = _size;
	auto ratio = _texture->getRatio();
	if (size.x / size.y > ratio)
		size.x = size.y * ratio;
	else
		size.y = size.x / ratio;
	return size * transform()->scale();
}

glm::vec2 RectRenderer::size() const { return _size; }
void RectRenderer::setSize(const glm::vec2& size) { this->_size = size; }
void RectRenderer::setPreserveAspect(bool preserveAspect) { _preserveAspect = preserveAspect; }

void RectRenderer::setNativeSize()
{
	glm::ivec2 size;
	SDL_QueryTexture(_texture->texture(), nullptr, nullptr, &size.x, &size.y);
	this->_size = size;
}
