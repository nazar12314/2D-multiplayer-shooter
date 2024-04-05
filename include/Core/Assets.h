// ReSharper disable CppMemberFunctionMayBeStatic
#pragma once

#include <string>
#include <vector>

#include "Color.h"
#include "Concepts.h"
#include "SDL2_ttf.h"

class Asset;

class Assets
{
	inline static std::vector<Asset*> assets {};

public:
	template <derived<Asset> T, typename... Ts> static T* load(const std::string& path, Ts... args);
};


class Asset
{
	std::string _path;

protected:
	Asset(const std::string& path) : _path(path) {}
	virtual ~Asset() = default;

	template <derived<Asset> T, typename... Ts> bool satisfies(const std::string& path, Ts... args) const;

public:
	const std::string& path() const { return _path; }

	friend class Assets;
};


class Sprite : public Asset
{
	SDL_Surface* _surface;
	SDL_Texture* _defaultTex;

	Sprite(const std::string& path);
	~Sprite() override;

public:
	bool satisfies() const { return true; }

	SDL_Surface* surface() const { return _surface; }
	SDL_Texture* defaultTexture() const { return _defaultTex; }

	friend class Assets;
};

class Font : public Asset
{
	TTF_Font* _font;
	int _size;

	Font(const std::string& path, int size);
	~Font() override;

public:
	bool satisfies(int size) const;

	TTF_Font* font() const { return _font; }

	friend class Assets;
};


template <derived<Asset> T, typename... Ts> bool Asset::satisfies(const std::string& path, Ts... args) const
{
	return path == _path && dynamic_cast<const T*>(this)->satisfies(args...);
}

template <derived<Asset> T, typename... Ts> T* Assets::load(const std::string& path, Ts... args)
{
	auto it = std::ranges::find_if(assets.begin(), assets.end(), [&](const Asset* asset) { return asset->satisfies<T>(path, args...); });
	if (it != assets.end())
		return dynamic_cast<T*>(*it);

	auto asset = new T(path, args...);
	assets.push_back(asset);
	return asset;
}
