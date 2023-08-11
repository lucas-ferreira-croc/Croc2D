#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <vector>
#include <set>
#include <deque>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include "../logger/logger.h"

const unsigned int MAX_COMPONENTS = 32;

////////////////////////////////////
// Signature
////////////////////////////////////
typedef std::bitset<MAX_COMPONENTS> Signature;

////////////////////////////////////
// Component
////////////////////////////////////

struct IComponent
{
protected:
	static int next_id;
};

template <typename T>
class Component : public IComponent
{
public:
	static int get_id() 
	{
		static auto id = next_id++; 
		return id;
	}

private:
	int id;
};

////////////////////////////////////
// Entity
////////////////////////////////////

class Registry;

class Entity
{
private:
	int id;
public:
	Entity(int id) : id(id) {}
	Entity(const Entity& entity) = default;
	void kill();
	int get_id() const;

	Entity& operator =(const Entity& e) = default;
	bool operator ==(const Entity& e) const {	return id == e.get_id(); }
	bool operator !=(const Entity& e) const { return id != e.get_id(); }
	bool operator >(const Entity& e) const { return id > e.get_id(); }
	bool operator <(const Entity& e) const { return id < e.get_id(); }

	Registry* registry;

	template<typename TComponent, typename ...TArgs> 
	void add_component(TArgs&& ...args);

	template<typename TComponent> 
	void remove_component();
	
	template<typename TComponent> 
	bool has_component() const;
	
	template<typename TComponent> 
	TComponent& get_component() const;
};

template<typename TComponent, typename ...TArgs> 
void Entity::add_component(TArgs&& ...args)
{
	this->registry->add_component<TComponent>(*this, std::forward<TArgs>(args)...);
}

template<typename TComponent>
void Entity::remove_component()
{
	this->registry->remove_component<TComponent>(*this);
}

template<typename TComponent> 
bool Entity::has_component() const
{
	return this->registry->has_component<TComponent>(*this);
}

template<typename TComponent> 
TComponent& Entity::get_component() const
{
	return this->registry->get_component<TComponent>(*this);
}

////////////////////////////////////
// System
////////////////////////////////////
class System 
{
private: 
	Signature component_signature;
	std::vector<Entity> entities;
public:
	System() = default;
	virtual ~System() = default;

	void add_entity(Entity entity);
	void remove_entity(Entity entity);
	std::vector<Entity> get_system_entities() const;
	const Signature& get_component_signature() const;

	template<typename TComponent>
	void require_component();
};


template <typename TComponent>
void System::require_component()
{
	const auto component_id = Component<TComponent>::get_id();
	component_signature.set(component_id);
}
////////////////////////////////////
// Pool
////////////////////////////////////
class IPool
{
public:
	virtual ~IPool() {}
};

template <typename T>
class Pool : public IPool
{
private:
	std::vector<T> data;

public:
	

	Pool(int size = 100) { data.resize(size); }
	virtual ~Pool() = default;

	bool is_empty() const { return data.empty(); }

	int get_size() const { return data.size(); }

	void resize(int n) { data.resize(n); }

	void clear() { data.clear(); }

	void add(T object) { data.push_back(object); }

	void set(int index, T object) { data[index] = object; }

	T& get(int index) { return static_cast<T&>(data[index]); }

	T& operator [](unsigned int index) { return data[index]; }
};


////////////////////////////////////
// Registry
////////////////////////////////////
class Registry
{
private:
	int num_entities = 0;
	
	//vector id = compnent type id
	//pool idnex = entity id
	std::vector<std::shared_ptr<IPool>> component_pools;
	
	// vector index = entity[id]
	std::vector<Signature> entity_component_signatures;

	std::unordered_map<std::type_index, std::shared_ptr<System>> systems;

	std::set<Entity> entities_to_be_added;
	std::set<Entity> entities_to_be_killed;

	std::deque<int> free_ids;
public:

	Registry() = default;

	void update();

	//entity management
	Entity create_entity();

	void kill_entity(Entity entity);

	//component management
	template <typename TComponent, typename ...TArgs>
	void add_component(Entity entity, TArgs&& ...args);

	template <typename TComponent>
	void remove_component(Entity entity);
	
	template <typename TComponent>
	bool has_component(Entity entity) const;
	
	template<typename TComponent> 
	TComponent& get_component(Entity entity) const;

	 //system management
	template <typename TSystem, typename...TArgs>
	void add_system(TArgs&&... args);

	template <typename TSystem>
	void remove_system();

	template <typename TSystem>
	bool has_system() const;

	template <typename TSystem>
	TSystem& get_system() const;

	void add_entity_to_system(Entity entity);
	void remove_entity_from_systems(Entity entity);
};


////////////////////////////
// component management
////////////////////////////
template <typename TComponent, typename ...TArgs>
void Registry::add_component(Entity entity, TArgs&& ...args)
{
	const int component_id = Component<TComponent>::get_id();
	const int entity_id = entity.get_id();

	if(component_id >= component_pools.size())
	{
		component_pools.resize(component_id + 1, nullptr);
	}

	if(!component_pools[component_id])
	{
		std::shared_ptr<Pool<TComponent>> new_component_pool = std::make_unique<Pool<TComponent>>();
		component_pools[component_id] = new_component_pool;
	}

	std::shared_ptr<Pool<TComponent>> component_pool = std::static_pointer_cast<Pool<TComponent>>(component_pools[component_id]);
	
	if(entity_id >= component_pool->get_size())
	{
		component_pool->resize(num_entities);
	}
	
	TComponent new_component(std::forward<TArgs>(args)...);
	component_pool->set(entity_id, new_component);

	entity_component_signatures[entity_id].set(component_id);
	
	Logger::log("component " + std::to_string(component_id) + "  was added to entity " + std::to_string(entity_id));
}

template <typename TComponent>
void Registry::remove_component(Entity entity)
{
	const int component_id = Component<TComponent>::get_id();
	const int entity_id = entity.get_id();

	entity_component_signatures[entity_id].set(entity_id, false);

	Logger::log("component id = " + std::to_string(component_id) + " was removed from entity id = " + std::to_string(entity_id));
}

template <typename TComponent>
bool Registry::has_component(Entity entity) const
{
	const int component_id = Component<TComponent>::get_id();
	const int entity_id = entity.get_id();
	return entity_component_signatures[entity_id].test(component_id);
}

template<typename TComponent>
TComponent& Registry::get_component(Entity entity) const
{
	int component_id = Component<TComponent>::get_id();
	int entity_id = entity.get_id();
	
	auto component_pool = std::static_pointer_cast<Pool<TComponent>>(component_pools[component_id]);

	return component_pool->get(entity_id);
}


////////////////////////////
// system management
////////////////////////////

template <typename TSystem, typename...TArgs>
void Registry::add_system(TArgs&&... args)
{
	std::shared_ptr<TSystem> new_system = std::make_shared<TSystem>(std::forward<TArgs>(args)...);
	systems.insert(std::make_pair(std::type_index(typeid(TSystem)), new_system));
}

template <typename TSystem>
void Registry::remove_system(){

	auto system = systems.find(std::type_index(typeid(TSystem)));
	systems.erase(system);
}

template <typename TSystem>
bool Registry::has_system() const
{
	return systems.find(std::type_index(typeid(TSystem))) != systems.end();
}

template <typename TSystem>
TSystem& Registry::get_system() const
{
	auto system = systems.find(std::type_index(typeid(TSystem)));
	return *(std::static_pointer_cast<TSystem>(system->second));
}

#endif
