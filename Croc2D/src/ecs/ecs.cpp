#include "ecs.h"

////////////////////////////////////
// Entity
////////////////////////////////////
int Entity::get_id() const
{
	return id;
}

////////////////////////////////////
// System
////////////////////////////////////
void System::add_entity(Entity entity)
{
	entities.push_back(entity);
}

void System::remove_entity(Entity entity)
{
	entities.erase(std::remove_if(entities.begin(), entities.end(),
		[&entity](Entity e) {
			return entity == e;
		}), entities.end());
}

std::vector<Entity> System::get_entities() const
{
	return entities;
}

const signature& System::get_component_signature() const
{
	return component_signature;

}
