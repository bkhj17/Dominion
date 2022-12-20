#include "framework.h"

Timer::Timer()
{
	//������ �ð������� ���� ���
	//QueryPerformanceFrequency : 1�ʵ����� cpu������ ��ȯ
	QueryPerformanceFrequency((LARGE_INTEGER*)&periodFrequency);
	//QueryPerformanceCounter : ���� CPU ������ ��ȯ
	QueryPerformanceCounter((LARGE_INTEGER*)&lastTime);

	timeScale = 1.0f / (float)periodFrequency;	//������ ���� ����. ������ ������ ���� ���ϱ� �������� �� ����
}

Timer::~Timer()
{
	
}

void Timer::Update()
{
	//(���� ������ - ���� �ð� ������) * 1/1�ʴ������� => ���� �ð�(�ʴ���)
	QueryPerformanceCounter((LARGE_INTEGER*)&curTime);
	elapsedTime = (float)(curTime - lastTime)*timeScale;
	lastTime = curTime;

	frameCount++;
	oneSecCount += elapsedTime;
	if (oneSecCount >= 1.0f) {	//1�� �̻� �����ٸ�
		frameRate = frameCount;
		frameCount = 0;
		oneSecCount = 0.0f;
	}
}

void Timer::Render(HDC hdc)
{
	wstring str = L"FPS : " + to_wstring(frameRate);
	TextOut(hdc, 0, 0, str.c_str(), (int)str.size());
	str = L"Delta : " + to_wstring(elapsedTime);
	TextOut(hdc, 0, 20, str.c_str(), (int)str.size());
}
