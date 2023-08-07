#pragma once

#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <SDL.h>

#include "../ecs.h"
#include "../../asset_store/asset_store.h"

#include "../components/transform.h"
#include "../components/sprite_component.h"

#include <algorithm>

class RenderSystem : public System
{
public:
	RenderSystem()
	{
		require_component<TransformComponent>();
		require_component<SpriteComponent>();
	}

	void update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& asset_store, SDL_Rect& camera)
	{
		auto renderable_entities = get_system_entities();
		std::sort(renderable_entities.begin(), renderable_entities.end(), [](const Entity& a, const Entity& b) {
			return a.get_component<SpriteComponent>().z_index < b.get_component<SpriteComponent>().z_index;
		});

		for(auto entity : renderable_entities)
		{
			const auto transform = entity.get_component<TransformComponent>();
			const auto sprite = entity.get_component<SpriteComponent>();

			SDL_Rect source_rectangle = sprite.src_rect;
			
			SDL_Rect destination_rectangle = {
				static_cast<int>(transform.position.x - camera.x),
				static_cast<int>(transform.position.y - camera.y),
				static_cast<int>(sprite.width * transform.scale.x),
				static_cast<int>(sprite.height * transform.scale.y)
			};

			SDL_RenderCopyEx(renderer, 
						     asset_store->get_texture(sprite.asset_id),
				             &source_rectangle,
						     &destination_rectangle,
				             transform.rotation,
						     NULL,
				             SDL_FLIP_NONE
			);
		}
	}
};

#endif // !RENDER_SYSTEM
