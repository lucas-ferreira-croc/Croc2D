#ifndef ASSET_STORE_H
#define ASSET_STORE_H

#include <map>
#include <string>
#include <SDL.h>

class AssetStore
{
private:
	std::map<std::string, SDL_Texture*> textures;
	std::map<std::string, SDL_Texture*> tile_map;
public:
	AssetStore();
	~AssetStore();

	void clear_assets();
	void add_texture(SDL_Renderer* renderer, const std::string& asset_id, const std::string& filepath);
	SDL_Texture* get_texture(const std::string& asset_id);
};

#endif // !ASSET_STORE_H
