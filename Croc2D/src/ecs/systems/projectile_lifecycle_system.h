#ifndef PROJECTILELIFECYCLESYSTEM_H
#define PROJECTILELIFECYCLESYSTEM_H

#include "../ecs.h"
#include "../components/projectile_component.h"

class ProjectileLifeCycleSystem : public System
{
public:
	ProjectileLifeCycleSystem()
	{
		require_component<ProjectileComponent>();
	}

	void update()
	{
		for(auto entity : get_system_entities())
		{
			auto projectile = entity.get_component<ProjectileComponent>();

			if(SDL_GetTicks() - projectile.start_time > projectile.duration)
			{
				entity.kill();
			}
		}
	}
};

#endif
