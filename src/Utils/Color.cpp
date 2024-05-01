#include "Color.h"

#include "MyMath.h"
#include "glm/common.hpp"
#include "glm/vec3.hpp"
#include "glm/geometric.hpp"

const Color Color::WHITE = {1.f, 1.f, 1.f};
const Color Color::BLACK = {0.f, 0.f, 0.f};
const Color Color::RED = {1.f, 0.f, 0.f};
const Color Color::GREEN = {0.f, 1.f, 0.f};
const Color Color::BLUE = {0.f, 0.f, 1.f};
const Color Color::YELLOW = {1.f, 1.f, 0.f};
const Color Color::CYAN = {0.f, 1.f, 1.f};
const Color Color::MAGENTA = {1.f, 0.f, 1.f};
const Color Color::GRAY = {0.2f, 0.2f, 0.2f};

const Color Color::DARK_GREEN = {0.f, 0.5f, 0.f};
const Color Color::SKY_BLUE = {.529f, .808f, .922f};
const Color Color::GOLD = {1.f, .843f, 0.f};
const Color Color::PINK = {255, 192, 203};
const Color Color::HOT_PINK = {255, 105, 180};

Color Color::random() { return {Math::randomValue(), Math::randomValue(), Math::randomValue()}; }

Color Color::randomLight()
{
	auto r = Math::randomValue();
	auto g = Math::randomValue();
	auto b = Math::randomValue();
	auto max = glm::max(r, glm::max(g, b));
	return {r / max, g / max, b / max};
}
Color Color::randomSaturated()
{
	auto r = Math::randomValue();
	auto g = Math::randomValue();
	auto b = Math::randomValue();
	return normalize(glm::vec3(r, g, b));
}

Color Color::withAlpha(float a) const { return {x, y, z, a}; }
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

Color Color::operator*(Color c) const { return {x * c.x, y * c.y, z * c.z, w * c.w}; }
Color Color::operator*(float v) const { return {x * v, y * v, z * v, w * v}; }

Color& Color::operator+=(Color c)
{
	x += c.x;
	y += c.y;
	z += c.z;
	return *this;
}
Color Color::operator+(Color c) const { return {x + c.x, y + c.y, z + c.z, w + c.w}; }

Color& Color::operator-=(Color c)
{
	x -= c.x;
	y -= c.y;
	z -= c.z;
	return *this;
}
Color Color::operator-(Color c) const { return {x - c.x, y - c.y, z - c.z, w - c.w}; }
bool Color::operator==(const Color& c) const { return x == c.x && y == c.y && z == c.z && w == c.w; }
bool Color::operator!=(const Color& c) const { return x != c.x || y != c.y || z != c.z || w != c.w; }

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
	auto a = c1.a() + (c2.a() - c1.a()) * value;
	return {r, g, b, a};
}
Color Color::darken(float factor) const
{
	Color hsv = ColorUtils::RGBtoHSV(*this);
	hsv.z -= factor;
	return ColorUtils::HSVtoRGB(hsv).withAlpha(w);
}
Color Color::lighten(float factor) const
{
	Color hsv = ColorUtils::RGBtoHSV(*this);
	hsv.z += factor;
	return ColorUtils::HSVtoRGB(hsv).withAlpha(w);
}
Color Color::saturate(float factor) const
{
	Color hsv = ColorUtils::RGBtoHSV(*this);
	hsv.y += factor;
	return ColorUtils::HSVtoRGB(hsv).withAlpha(w);
}
Color Color::desaturate(float factor) const
{
	Color hsv = ColorUtils::RGBtoHSV(*this);
	hsv.y -= factor;
	return ColorUtils::HSVtoRGB(hsv).withAlpha(w);
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

Color ColorUtils::RGBtoHSV(const Color& in)
{
	float r = in.r();
	float g = in.g();
	float b = in.b();

	Color out = {0, 0, 0};
	float& h = out.x;
	float& s = out.y;
	float& v = out.z;

	auto fCMax = std::max(std::max(r, g), b);
	auto fCMin = std::min(std::min(r, g), b);
	auto fDelta = fCMax - fCMin;

	if (fDelta > 0)
	{
		if (fCMax == r)
			h = 60 * (fmod(((g - b) / fDelta), 6));
		else if (fCMax == g)
			h = 60 * (((b - r) / fDelta) + 2);
		else if (fCMax == b)
			h = 60 * (((r - g) / fDelta) + 4);

		if (fCMax > 0)
			s = fDelta / fCMax;
		else
			s = 0;

		v = fCMax;
	}
	else
	{
		h = 0;
		s = 0;
		v = fCMax;
	}

	if (h < 0)
		h = 360 + h;

	return out;
}
Color ColorUtils::HSVtoRGB(const Color& in)
{
	float h = in.x;
	float s = in.y;
	float v = in.z;

	Color out = {0, 0, 0};
	float& r = out.x;
	float& g = out.y;
	float& b = out.z;

	auto fC = v * s; // Chroma
	auto fHPrime = (float)fmod(h / 60.0f, 6);
	auto fX = fC * (1 - (float)fabs(fmod(fHPrime, 2) - 1));
	auto fM = v - fC;

	if (0 <= fHPrime && fHPrime < 1)
	{
		r = fC;
		g = fX;
		b = 0;
	}
	else if (1 <= fHPrime && fHPrime < 2)
	{
		r = fX;
		g = fC;
		b = 0;
	}
	else if (2 <= fHPrime && fHPrime < 3)
	{
		r = 0;
		g = fC;
		b = fX;
	}
	else if (3 <= fHPrime && fHPrime < 4)
	{
		r = 0;
		g = fX;
		b = fC;
	}
	else if (4 <= fHPrime && fHPrime < 5)
	{
		r = fX;
		g = 0;
		b = fC;
	}
	else if (5 <= fHPrime && fHPrime < 6)
	{
		r = fC;
		g = 0;
		b = fX;
	}
	else
	{
		r = 0;
		g = 0;
		b = 0;
	}

	r += fM;
	g += fM;
	b += fM;

	return out;
}

Color operator*(float v, Color c) { return {c.x * v, c.y * v, c.z * v, c.w * v}; }
Color operator*(Color c, float v) { return {c.x * v, c.y * v, c.z * v, c.w * v}; }
