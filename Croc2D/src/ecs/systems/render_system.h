#pragma once

#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <SDL.h>

#include "../ecs.h"
#include "../components/transform.h"
#include "../components/sprite_component.h"


class RenderSystem : public System
{
public:
	RenderSystem()
	{
		require_component<TransformComponent>();
		require_component<SpriteComponent>();
	}

	void update(SDL_Renderer* renderer)
	{
		for(auto entity : get_system_entities())
		{
			const auto transform = entity.get_component<TransformComponent>();
			const auto sprite = entity.get_component<SpriteComponent>();

			SDL_Rect objct_rect = {
				static_cast<int>(transform.position.x),
				static_cast<int>(transform.position.y),
				sprite.width,
				sprite.height
			};

			SDL_SetRenderDrawColor(renderer, 255, 0, 255, 1);
			SDL_RenderDrawRect(renderer, &objct_rect);
		}
	}
};

#endif // !RENDER_SYSTEM
