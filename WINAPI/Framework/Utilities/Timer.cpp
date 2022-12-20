#include "framework.h"

Timer::Timer()
{
	//정밀한 시간관리를 위해 사용
	//QueryPerformanceFrequency : 1초동안의 cpu진동수 반환
	QueryPerformanceFrequency((LARGE_INTEGER*)&periodFrequency);
	//QueryPerformanceCounter : 현재 CPU 진동수 반환
	QueryPerformanceCounter((LARGE_INTEGER*)&lastTime);

	timeScale = 1.0f / (float)periodFrequency;	//나눠줄 값의 역수. 나누기 여러번 보다 곱하기 여러번이 더 빠름
}

Timer::~Timer()
{
	
}

void Timer::Update()
{
	//(현재 진동수 - 지난 시간 진동수) * 1/1초당진동수 => 지난 시간(초단위)
	QueryPerformanceCounter((LARGE_INTEGER*)&curTime);
	elapsedTime = (float)(curTime - lastTime)*timeScale;
	lastTime = curTime;

	frameCount++;
	oneSecCount += elapsedTime;
	if (oneSecCount >= 1.0f) {	//1초 이상 지났다면
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
