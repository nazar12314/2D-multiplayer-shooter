#include "BaseRenderer.h"

#include "Camera.h"
#include "Material.h"
#include "Renderer.h"
#include "SDLHandler.h"
#include "Transform.h"

BaseRenderer::BaseRenderer(GameObject* obj, glm::vec2 size, const Color& color, int order) : Component(obj), _size(size), _color(color), _order(order) {}
BaseRenderer::~BaseRenderer()
{
	delete _material;
}
void BaseRenderer::render(const Camera* camera)
{
	auto texture = _material->sdlTexture();
	if (texture == nullptr) return;

	auto pos = transform()->getPos();
	auto screenPos = camera->worldToScreenPoint(pos);
	auto sizeOnScreen = getFinalSize() / (glm::vec2)camera->size() * (float)SDLHandler::windowSize.y;
	Renderer::renderTex(texture, screenPos, sizeOnScreen, transform()->getRot() - camera->transform()->getRot());
}

glm::vec2 BaseRenderer::getFinalSize() const
{
	if (!_preserveAspect) return _size;

	auto ratio = _material->texture().getRatio();
	auto size = _size;
	if (size.x / size.y > ratio)
		size.x = size.y * ratio;
	else
		size.y = size.x / ratio;
	return size;
}

Material* BaseRenderer::material() const { return _material; }
int BaseRenderer::order() const { return _order; }
glm::vec2 BaseRenderer::size() const { return _size; }


void BaseRenderer::setSize(const glm::vec2& size) { this->_size = size; }
void BaseRenderer::setOrder(int order)
{
	this->_order = order;
	Renderer::sortRenderers();
}
void BaseRenderer::setColor(const Color& color)
{
	this->_color = color;
	_material->setColor(color);
}
void BaseRenderer::setPreserveAspect(bool preserveAspect)
{
	_preserveAspect = preserveAspect;
}

void BaseRenderer::setNativeSize()
{
	glm::ivec2 size;
	SDL_QueryTexture(_material->sdlTexture(), nullptr, nullptr, &size.x, &size.y);
	this->_size = size;
}
