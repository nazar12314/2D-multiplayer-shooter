#include "Color.h"

#include "glm/common.hpp"
#include "glm/vec3.hpp"
#include "glm/geometric.hpp"

Color Color::white = {1.f, 1.f, 1.f};
Color Color::black = {0.f, 0.f, 0.f};
Color Color::red = {1.f, 0.f, 0.f};
Color Color::green = {0.f, 1.f, 0.f};
Color Color::blue = {0.f, 0.f, 1.f};
Color Color::yellow = {1.f, 1.f, 0.f};
Color Color::cyan = {0.f, 1.f, 1.f};
Color Color::magenta = {1.f, 0.f, 1.f};
Color Color::gray = {0.2f, 0.2f, 0.2f};

Color Color::dark_green = {0.f, 0.5f, 0.f};
Color Color::sky_blue = {.529f, .808f, .922f};
Color Color::gold = {1.f, .843f, 0.f};
Color Color::pink = {255, 192, 203};
Color Color::hot_pink = {255, 105, 180};

Color Color::random()
{
	return {static_cast<float>(rand() % 256) / 255, static_cast<float>(rand() % 256) / 255, static_cast<float>(rand() % 256) / 255};
}

Color Color::randomLight()
{
	auto r = static_cast<float>(rand() % 256) / 255;
	auto g = static_cast<float>(rand() % 256) / 255;
	auto b = static_cast<float>(rand() % 256) / 255;
	auto max = glm::max(r, glm::max(g, b));
	return {r / max, g / max, b / max};
}
Color Color::randomSaturated()
{
	auto r = static_cast<float>(rand() % 256) / 255;
	auto g = static_cast<float>(rand() % 256) / 255;
	auto b = static_cast<float>(rand() % 256) / 255;
	return glm::normalize(glm::vec3(r, g, b));
}

Color Color::withAlpha(float a) const
{
	return {x, y, z, a};
}
Color& Color::operator*=(float v)
{
	x *= v;
	y *= v;
	z *= v;
	return *this;
}
Color& Color::operator*=(Color c)
{
	x *= c.x;
	y *= c.y;
	z *= c.z;
	return *this;
}

Color Color::operator*(Color c) const { return {x * c.x, y * c.y, z * c.z}; }
Color Color::operator*(float v) const
{
	return {x * v, y * v, z * v};
}

Color& Color::operator+=(Color c)
{
	x += c.x;
	y += c.y;
	z += c.z;
	return *this;
}
Color Color::operator+(Color c) const { return {x + c.x, y + c.y, z + c.z}; }

Color& Color::operator-=(Color c)
{
	x -= c.x;
	y -= c.y;
	z -= c.z;
	return *this;
}
Color Color::operator-(Color c) const { return {x - c.x, y - c.y, z - c.z}; }
bool Color::operator==(const Color& c) const
{
	return x == c.x && y == c.y && z == c.z && w == c.w;
}
bool Color::operator!=(const Color& c) const
{
	return x != c.x || y != c.y || z != c.z || w != c.w;
}

uint32_t Color::toColor32() const
{
	return (static_cast<int>(glm::clamp(a(), 0.0f, 1.0f) * 255) << 24) +
		(static_cast<int>(glm::clamp(r(), 0.0f, 1.0f) * 255) << 16) +
		(static_cast<int>(glm::clamp(g(), 0.0f, 1.0f) * 255) << 8) +
		(static_cast<int>(glm::clamp(b(), 0.0f, 1.0f) * 255) << 0);
}
Color Color::fromColor32(uint32_t color32)
{
	auto a = static_cast<float>(color32 >> 24 & 0xFF) / 255;
	auto r = static_cast<float>(color32 >> 16 & 0xFF) / 255;
	auto g = static_cast<float>(color32 >> 8 & 0xFF) / 255;
	auto b = static_cast<float>(color32 >> 0 & 0xFF) / 255;
	return {r, g, b, a};
}

Color Color::lerp(const Color& c1, const Color& c2, float value)
{
	auto r = c1.r() + (c2.r() - c1.r()) * value;
	auto g = c1.g() + (c2.g() - c1.g()) * value;
	auto b = c1.b() + (c2.b() - c1.b()) * value;
	return {r, g, b};
}
Color Color::darken(float factor) const
{
	return {x * (1 - factor), y * (1 - factor), z * (1 - factor)};
}
Color Color::lighten(float factor) const
{
	return {x + (1 - x) * factor, y + (1 - y) * factor, z + (1 - z) * factor};
}

SDL_Color Color::toSDLColor() const
{
	return {
		static_cast<Uint8>(glm::clamp(r(), 0.0f, 1.0f) * 255),
		static_cast<Uint8>(glm::clamp(g(), 0.0f, 1.0f) * 255),
		static_cast<Uint8>(glm::clamp(b(), 0.0f, 1.0f) * 255),
		static_cast<Uint8>(glm::clamp(a(), 0.0f, 1.0f) * 255)
	};
}

Color operator*(float v, Color c)
{
	return {c.x * v, c.y * v, c.z * v};
}
Color operator*(Color c, float v)
{
	return {c.x * v, c.y * v, c.z * v};
}
