#pragma once

#include <ostream>

#include "SDL_pixels.h"
#include "glm/vec3.hpp"

struct Color
{
	float x, y, z, w;

	Color(float r, float g, float b, float a = 1) : x(r), y(g), z(b), w(a) {}
	Color(int r, int g, int b, int a = 255) : x(static_cast<float>(r) / 255.f), y(static_cast<float>(g) / 255.f), z(static_cast<float>(b) / 255.f),
	                                          w(static_cast<float>(a) / 255.f) {}
	Color(glm::vec3 rgb, float a = 1) : x(rgb.x), y(rgb.y), z(rgb.z), w(a) {}
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

	static Color random();
	static Color randomLight();
	static Color randomSaturated();

	float r() const { return x; }
	float g() const { return y; }
	float b() const { return z; }
	float a() const { return w; }

	Color withAlpha(float a) const;

	Color& operator*=(float v);
	Color& operator*=(Color c);

	Color operator*(float v) const;
	Color operator*(Color c) const;

	Color& operator+=(Color c);
	Color operator+(Color c) const;

	Color& operator-=(Color c);
	Color operator-(Color c) const;

	bool operator==(const Color& c) const;
	bool operator!=(const Color& c) const;

	uint32_t toColor32() const;
	static Color fromColor32(uint32_t color32);
	static Color lerp(const Color& c1, const Color& c2, float value);

	SDL_Color toSDLColor() const;
};


inline std::ostream& operator<<(std::ostream& os, const Color& c);
