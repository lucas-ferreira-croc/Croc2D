#include "ecs.h"
#include "../logger/logger.h"

int IComponent::next_id = 0;

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

std::vector<Entity> System::get_system_entities() const
{
	return entities;
}

const Signature& System::get_component_signature() const
{
	return component_signature;

}

////////////////////////////////////
// Registry
////////////////////////////////////
Entity Registry::create_entity()
{
	int entity_id;
	entity_id = num_entities++;

	Entity entity(entity_id);
	entity.registry = this;
	entities_to_be_added.insert(entity);
	
	if(entity_id >= entity_component_signatures.size())
	{
		entity_component_signatures.resize(entity_id + 1);
	}

	Logger::log("Entity created with id = " + std::to_string(entity_id));
	return entity;
}

void Registry::update()
{
	for(Entity entity : entities_to_be_added)
	{
		add_entity_to_system(entity);
	}
	entities_to_be_added.clear();

	/*for(Entity entity : entities_to_be_killed)
	{

	}*/
}

void Registry::add_entity_to_system(Entity entity)
{
	const int entity_id = entity.get_id();
	
	const auto& component_signature = entity_component_signatures[entity_id];

	for (auto& system: systems)
	{
		const auto& system_component_signature = system.second->get_component_signature();
		
		bool is_interested = (component_signature & system_component_signature) == system_component_signature;
		if (is_interested)
		{
			system.second->add_entity(entity);
		}
	}
}