#include "ecs.h"
#include "../logger/logger.h"

int IComponent::next_id = 0;

void Entity::kill()
{
	registry->kill_entity(*this);
}

////////////////////////////////////
// Entity
////////////////////////////////////
int Entity::get_id() const
{
	return id;
}

void Entity::tag(const std::string& tag)
{
	registry->tag_entity(*this, tag);
}

bool Entity::has_tag(const std::string& tag) const
{
	return registry->entity_has_tag(*this, tag);
}

void Entity::group(const std::string& group)
{
	registry->group_entity(*this, group);
}

bool Entity::belongs_to_group(const std::string& group) const
{
	return registry->entity_belongs_to_group(*this, group);
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

	if(free_ids.empty())
	{
		entity_id = num_entities++;
		if(entity_id >= entity_component_signatures.size())
		{
			entity_component_signatures.resize(entity_id + 1);
		}
	}
	else
	{
		entity_id = free_ids.front();
		free_ids.pop_front();
	}

	Entity entity(entity_id);
	entity.registry = this;
	entities_to_be_added.insert(entity);
	
	Logger::log("Entity created with id = " + std::to_string(entity_id));
	return entity;
}

void Registry::kill_entity(Entity entity)
{
	entities_to_be_killed.insert(entity);
}

void Registry::update()
{
	for(Entity entity : entities_to_be_added)
	{
		add_entity_to_system(entity);
	}
	entities_to_be_added.clear();

	for(Entity entity : entities_to_be_killed)
	{
		remove_entity_from_systems(entity);
		
		free_ids.push_back(entity.get_id());
		
		entity_component_signatures[entity.get_id()].reset();
		remove_entity_tag(entity);
		remove_entity_group(entity);

	}
	entities_to_be_killed.clear();
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

void Registry::remove_entity_from_systems(Entity entity)
{
	for(auto system : systems)
	{
		system.second->remove_entity(entity);
	}
}

void Registry::tag_entity(Entity entity, const std::string& tag)
{
	entity_per_tag.emplace(tag, entity);
	tag_per_entity.emplace(entity.get_id(), tag);
}

bool Registry::entity_has_tag(Entity entity, const std::string& tag) const
{
	if (tag_per_entity.find(entity.get_id()) == tag_per_entity.end())
		return false;
	return entity_per_tag.find(tag)->second == entity;
}

Entity Registry::get_entity_by_tag(const std::string& tag)
{
	return entity_per_tag.at(tag);
}

void Registry::remove_entity_tag(Entity entity)
{
	auto tagged_entity = tag_per_entity.find(entity.get_id());
	if(tagged_entity != tag_per_entity.end())
	{
		auto tag = tagged_entity->second;
		entity_per_tag.erase(tag);
		tag_per_entity.erase(tagged_entity);
	}
}

void Registry::group_entity(Entity entity, const std::string& group)
{
	entities_per_group.emplace(group, std::set<Entity>());
	entities_per_group[group].emplace(entity);
	group_per_entity.emplace(entity.get_id(), group);
}

bool Registry::entity_belongs_to_group(Entity entity, const std::string& group) const
{
	if (entities_per_group.find(group) == entities_per_group.end())
		return false;

	auto group_entities = entities_per_group.at(group);	
	return group_entities.find(entity.get_id()) != group_entities.end();
}

std::vector<Entity> Registry::get_entities_by_group(const std::string group)
{
	auto& set_of_entities = entities_per_group.at(group);
	return std::vector<Entity>(set_of_entities.begin(), set_of_entities.end());
}

void Registry::remove_entity_group(Entity entity)
{
	auto grouped_entity = group_per_entity.find(entity.get_id());
	if(grouped_entity != group_per_entity.end())
	{
		auto group = entities_per_group.find(grouped_entity->second);
		if(group != entities_per_group.end())
		{
			auto entity_in_group = group->second.find(entity);
			if(entity_in_group != group->second.end())
			{
				group->second.erase(entity_in_group);
			}
		}
		group_per_entity.erase(grouped_entity);
	}
}
