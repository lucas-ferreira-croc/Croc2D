#include "asset_store.h"
#include "../logger/logger.h"

#include <SDL_image.h>
AssetStore::AssetStore()
{
}

AssetStore::~AssetStore()
{
	clear_assets();
}

void AssetStore::clear_assets()
{
	for(auto texture : textures)
	{
		SDL_DestroyTexture(texture.second);
	}
}

void AssetStore::add_texture(SDL_Renderer* renderer, const std::string& asset_id, const std::string& filepath)
{
	SDL_Surface* surface = IMG_Load(filepath.c_str());
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	textures.emplace(asset_id, texture);

	Logger::log("new asset added with id " + asset_id);
}

SDL_Texture* AssetStore::get_texture(const std::string& asset_id)
{
	return textures.at(asset_id);
}
