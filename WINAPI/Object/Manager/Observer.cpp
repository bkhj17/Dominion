#include "framework.h"
#include "Observer.h"

void Observer::AddEvent(string key, Event event)
{
	totalEvent[key].push_back(event);
}

void Observer::ExcuteEvents(string key)
{
	if (totalParamEvent.count(key) == 0)
		return;

	for (const auto& event : totalEvent[key])
		event();
}

void Observer::AddParamEvent(string key, ParamEvent event)
{
	totalParamEvent[key].push_back(event);
}

void Observer::ExcuteParamEvents(string key, void* object)
{
	if (totalParamEvent.count(key) == 0)
		return;
	
	for (const auto& event : totalParamEvent[key]) {
		event(object);
	}
}
