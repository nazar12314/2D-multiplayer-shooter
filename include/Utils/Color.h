#pragma once

#include <ostream>
#include <glm/common.hpp>
#include <glm/vec4.hpp>

class Color : public glm::vec4
{
public:
	Color(float r, float g, float b, float a = 1) : glm::vec4(r, g, b, a) {}
	Color(int r, int g, int b, int a = 255) : glm::vec4(static_cast<float>(r) / 255.f, static_cast<float>(g) / 255.f, static_cast<float>(b) / 255.f,
	                                                    static_cast<float>(a) / 255.f) {}
	Color() : Color {0, 0, 0, 0} {}

	static Color white;
	static Color black;
	static Color red;
	static Color green;
	static Color blue;
	static Color yellow;
	static Color cyan;
	static Color magenta;
	static Color gray;

	static Color dark_green;
	static Color sky_blue;
	static Color gold;
	static Color pink;
	static Color hot_pink;

	float r() const { return x; }
	float g() const { return y; }
	float b() const { return z; }
	float a() const { return w; }

	Color& operator*=(float v)
	{
		x *= v;
		y *= v;
		z *= v;
		return *this;
	}
	Color& operator*=(Color c)
	{
		x *= c.x;
		y *= c.y;
		z *= c.z;
		return *this;
	}

	Color operator*(Color c) const { return {x * c.x, y * c.y, z * c.z}; }

	Color& operator+=(Color c)
	{
		x += c.x;
		y += c.y;
		z += c.z;
		return *this;
	}
	Color operator+(Color c) const { return {x + c.x, y + c.y, z + c.z}; }

	uint32_t toColor32() const
	{
		return (static_cast<int>(glm::clamp(a(), 0.0f, 1.0f) * 255) << 24) +
			(static_cast<int>(glm::clamp(r(), 0.0f, 1.0f) * 255) << 16) +
			(static_cast<int>(glm::clamp(g(), 0.0f, 1.0f) * 255) << 8) +
			(static_cast<int>(glm::clamp(b(), 0.0f, 1.0f) * 255) << 0);
	}
	static Color fromColor32(uint32_t color32)
	{
		auto a = static_cast<float>(color32 >> 24 & 0xFF) / 255;
		auto r = static_cast<float>(color32 >> 16 & 0xFF) / 255;
		auto g = static_cast<float>(color32 >> 8 & 0xFF) / 255;
		auto b = static_cast<float>(color32 >> 0 & 0xFF) / 255;
		return {r, g, b, a};
	}

	static Color lerp(const Color& c1, const Color& c2, float value)
	{
		auto r = c1.r() + (c2.r() - c1.r()) * value;
		auto g = c1.g() + (c2.g() - c1.g()) * value;
		auto b = c1.b() + (c2.b() - c1.b()) * value;
		return {r, g, b};
	}
};


inline Color operator*(float v, Color c) { return {c.x * v, c.y * v, c.z * v}; }
inline Color operator*(Color c, float v) { return {c.x * v, c.y * v, c.z * v}; }

inline std::ostream& operator<<(std::ostream& os, const Color& c)
{
	os << "(" << c.r() << ", " << c.g() << ", " << c.b() << ")";
	return os;
}
