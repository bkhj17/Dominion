#include "framework.h"

void KeyBoard::Update()
{
	memcpy(oldState, curState, sizeof(oldState));

	//0, 1		-> 키보드 입력 x
	//128, 129	-> 키보드 입력 o
	GetKeyboardState(curState);

	for (UINT i = 0; i < KEY_MAX; i++) {
		BYTE key = curState[i] & 0x80;
		curState[i] = key ? 1 : 0;

		BYTE old = oldState[i];
		BYTE curCard = curState[i];

		if (old == 0 && curCard == 1)
			mapState[i] = DOWN;
		else if (old == 1 && curCard == 0)
			mapState[i] = UP;
		else if (old == 1 && curCard == 1)
			mapState[i] = PRESS;
		else
			mapState[i] = NONE;
	}
}
