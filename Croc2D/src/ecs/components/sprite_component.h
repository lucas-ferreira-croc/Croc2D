#pragma once

#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H

#include "../ecs.h"
#include <string>
#include <SDL.h>

struct SpriteComponent
{
	std::string asset_id;
	int width;
	int height;
	int z_index;
	bool is_fixed;
	SDL_Rect src_rect;

	SpriteComponent(const std::string asset_id = "", int width = 0, int height = 0, int z_index = 0, int src_rect_x = 0, int src_rect_y = 0, bool is_fixed = false) 
		: asset_id(asset_id), width(width), height(height), src_rect({ src_rect_x, src_rect_y, width, height}), z_index(z_index), is_fixed(is_fixed) {}
};
#endif // !SPRITE_COMPONENT_h
