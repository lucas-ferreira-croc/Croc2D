#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <vector>

const unsigned int MAX_COMPONENTS = 32;

////////////////////////////////////
// Signature
////////////////////////////////////
typedef std::bitset<MAX_COMPONENTS> signature;

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
class Entity
{
private:
	int id;
public:
	Entity(int id) : id(id) {}
	Entity(const Entity& entity) = default;
	int get_id() const;

	Entity& operator =(const Entity& e) = default;
	bool operator ==(const Entity& e) const {	return id == e.get_id(); }
	bool operator !=(const Entity& e) const { return id != e.get_id(); }
	bool operator >(const Entity& e) const { return id > e.get_id(); }
	bool operator <(const Entity& e) const { return id < e.get_id(); }
};

////////////////////////////////////
// System
////////////////////////////////////
class System 
{
private: 
	signature component_signature;
	std::vector<Entity> entities;
public:
	System() = default;
	virtual ~System() = default;

	void add_entity(Entity entity);
	void remove_entity(Entity entity);
	std::vector<Entity> get_entities() const;
	const signature& get_component_signature() const;

	template<typename TComponent>
	void require_component();
};

////////////////////////////////////
// Registry
////////////////////////////////////
class Registry
{
};


template <typename TComponent>
void System::require_component()
{
	const auto component_id = Component<TComponent>::get_id();
	component_signature.set(component_id);
}
#endif
