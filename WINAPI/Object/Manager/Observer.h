#pragma once
class Observer : public Singleton<Observer>
{
private:
	friend class Singleton;
	Observer() = default;
	~Observer() = default;
public:

	void AddEvent(string key, Event event);
	void ExcuteEvents(string key);

	void AddParamEvent(string key, ParamEvent event);
	void ExcuteParamEvents(string key, void* object);

private:
	//void 포인터 : 자료형이 정해지지 않았을 때 사용하는 포인터
	//어떤 포인터건 간에 집어넣을 수 있다
	//다시 되돌릴 때에는 원래 자료형으로 형변환

	typedef vector<Event> Events;
	typedef vector<ParamEvent> ParamEvents;


	map<string, Events> totalEvent;
	map<string, ParamEvents> totalParamEvent;
};

