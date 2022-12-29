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
	//void ������ : �ڷ����� �������� �ʾ��� �� ����ϴ� ������
	//� �����Ͱ� ���� ������� �� �ִ�
	//�ٽ� �ǵ��� ������ ���� �ڷ������� ����ȯ

	typedef vector<Event> Events;
	typedef vector<ParamEvent> ParamEvents;


	map<string, Events> totalEvent;
	map<string, ParamEvents> totalParamEvent;
};

