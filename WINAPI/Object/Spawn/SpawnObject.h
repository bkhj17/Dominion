#pragma once
class SpawnObject : public Rect
{
private:
	//UINT�� windows���� �̹� ���ǵǾ� �ִ�
	const float MIN_SIZE = 50.0f;
	const float MAX_SIZE = 200.0f;
	
	const float MIN_LIFE_TIME = 3.0f;
	const float MAX_LIFE_TIME = 10.0f;
public:
	SpawnObject();
	~SpawnObject();

	void Init();
	void Update();
	void Render(HDC hdc);

	void Spawn();
	void Destroy();
private:
	float lifeTime = 0.0f;	//�����ð�
	float playTime = 0.0f;	//����ð�

	HBRUSH hBrush = nullptr;
};

