#pragma once
class Table : public Singleton<Table>
{
	friend class Singleton;

public:
	void Init(int playerNum);



};

