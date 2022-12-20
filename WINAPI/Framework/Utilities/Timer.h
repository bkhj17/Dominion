#pragma once
class Timer :
    public Singleton<Timer>
{
    friend class Singleton;
private:
    Timer();
    ~Timer();

public:
    void Update();
    void Render(HDC hdc);

    float GetElapsedTime() { return elapsedTime; } //타이머 시간 간격


private:
    float timeScale = 0.0f;
    float elapsedTime = 0.0f;
    float oneSecCount = 0.0f;

    __int64 curTime = 0;
    __int64 lastTime = 0;
    __int64 periodFrequency = 0;

    int frameRate = 0;
    int frameCount = 0;
};

