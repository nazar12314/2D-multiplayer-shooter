#pragma once

#include <SDL_render.h>
#include <string>
#include <vector>

#include "Color.h"
#include "Concepts.h"

class Asset
{
	std::string _path;

protected:
	Asset(const std::string& path) : _path(path) {}
	virtual ~Asset() = default;

public:
	const std::string& path() const { return _path; }

	friend class Assets;
};

class Assets
{
	inline static std::vector<Asset*> assets {};

	static void init();

public:
	template <derived<Asset> T, typename... Ts> static T* load(const std::string& path, Ts... args);

	friend class Application;
};

template <derived<Asset> T, typename... Ts> T* Assets::load(const std::string& path, Ts... args)
{
	auto it = std::ranges::find_if(assets.begin(), assets.end(), [&](const Asset* asset) { return asset->path() == path; });
	if (it != assets.end())
		return dynamic_cast<T*>(*it);

	auto asset = new T(path, args...);
	assets.push_back(asset);
	return asset;
}

class TextureBase : public Asset
{
	SDL_Surface* _surface;

	TextureBase(const std::string& path);
	~TextureBase() override;

public:
	SDL_Surface* surface() const { return _surface; }

	friend class Assets;
};

class Texture
{
	TextureBase* _base;
	SDL_Texture* _texture;

	void setColor(const Color& color) const;

public:
	Texture(TextureBase* base);
	Texture(const std::string& path);
	Texture(const Texture& other);

	SDL_Texture* texture() const { return _texture; }

	friend class SpriteRenderer;
};
