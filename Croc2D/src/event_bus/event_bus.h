#ifndef EVENT_BUS_H
#define EVENT_BUS_H

#include <map>
#include <typeindex>
#include <list>
#include "event.h"

class IEventCallback
{
private:
	virtual void call(Event& e) = 0;
public:
	virtual ~IEventCallback() = default;

	void execute(Event& e)
	{
		call(e);
	}
};

template<typename TOwner, typename TEvent>
class EventCallback : public IEventCallback
{
private:
	typedef void (TOwner::*CallBackFunction)(TEvent&);

	TOwner* owner_instance;
	CallBackFunction callback_function;

	virtual void call(Event& e) override
	{
		std::invoke(callback_function, owner_instance, static_cast<TEvent&>(e));
	}

public:
	EventCallback(TOwner* owner_instance, CallBackFunction callback_function)
		: owner_instance(owner_instance), callback_function(callback_function)
	{
	}

	virtual ~EventCallback() override = default;
};

typedef std::list<std::unique_ptr<IEventCallback>> HandlerList;

class EventBus
{
private:
	std::map<std::type_index, std::unique_ptr<HandlerList>> subscribers;

public:
	EventBus() = default;

	~EventBus() = default;

	void reset()
	{
		subscribers.clear();
	}

	template<typename TEvent, typename TOwner>
	void subscribe_to_event(TOwner* owner_instance, void (TOwner::*callback_function)(TEvent&))
	{
		if(!subscribers[typeid(TEvent)].get())
		{
			subscribers[typeid(TEvent)] = std::make_unique<HandlerList>();
		}

		auto subscriber = std::make_unique<EventCallback<TOwner, TEvent>>(owner_instance, callback_function);
		subscribers[typeid(TEvent)]->push_back(std::move(subscriber));
	}

	template<typename TEvent, typename ...TArgs>
	void emit_event(TArgs&& ...args)
	{
		auto handlers = subscribers[typeid(TEvent)].get();

		if(handlers)
		{
			for(auto it = handlers->begin(); it != handlers->end(); it++)
			{
				auto handler = it->get();
				TEvent event(std::forward<TArgs>(args)...);
				handler->execute(event);
			}
		}
	}
};

#endif
