#include "framework.h"

Animation::Animation(POINT imageFrame, float speed)
	: imageFrame(imageFrame), speed(speed)
{
	intervalTime = 1.0f / speed;

	frames.reserve(imageFrame.x * imageFrame.y);
	for (int y = 0; y < imageFrame.y; y++) {
		for (int x = 0; x < imageFrame.x; x++) {
			frames.push_back({ x, y });
		}
	}


}

void Animation::Update()
{
	if (!isPlay)
		return;

	playTime += speed * DELTA;

	if (playTime >= intervalTime) {
		playTime -= intervalTime;

 		if (++curFrameNum >= action.size())
		{
			if(isLoop)
				curFrameNum = 0;
			else {
				curFrameNum--;
				Stop();
			}
		}
	}
}


void Animation::SetDefault(bool loop, bool pingpong)
{
	isLoop = loop;
	action.clear();

	for (UINT i = 0; i < frames.size(); i++) {
		action.push_back(i);
	}

	if (pingpong)
		SetPingpong();
}

void Animation::SetPart(UINT start, UINT end, bool loop, bool pingpong)
{
	isLoop = loop;
	action.clear();

	for (UINT i = start; i <= end; i++) {
		action.push_back(i);
	}

	if (pingpong)
		SetPingpong();
}

void Animation::SetPingpong()
{
	for (int i = (int)action.size() - 2; i >= 0; i--) {
		action.push_back(action[i]);
	}
}

void Animation::Stop()
{
	isPlay = false;
	if(endEvent)
		endEvent();
}
