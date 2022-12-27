#pragma once
class Animation
{
private:
	const float FRAME_PER_SECOND = 3.0f;

public:
	Animation(POINT imageFrame, float speed = 3.0f);
	~Animation() = default;

	void Update();

	void SetDefault(bool loop = true, bool pingpong = false);
	void SetPart(UINT start, UINT end, bool loop = true, bool pingpong = false);
	void SetArray(int* arr, int arrSize, bool loop = false, bool pingpong = false);
	void SetPingpong();

	void Play() { isPlay = true, curFrameNum = 0; }
	void Stop(); 

	void SetSpeed(float speed) { this->speed = speed; }

	bool IsPlay() {	return isPlay; }
	POINT GetFrame() { return { frames[action[curFrameNum]] }; }

	void SetEndEvent(function<void()> func) { 
		endEvent = func; 
	}

	void AttactEnd() { attack = false; }

private:
	vector<POINT> frames;
	vector<int> action;

	int curFrameNum = 0;
	POINT imageFrame;

	bool isPlay = false;
	bool isLoop = false;

	float playTime = 0.0f;
	float speed;
	float intervalTime = 0.0f;

	function<void()> endEvent;

	bool attack = false;

};