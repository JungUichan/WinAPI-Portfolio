#pragma once
#include <bitset>
#include "Singletonbase.h"
/****************************************************************************
## Input ##
@@ Author : 권순우 , Date : 2019.03.20
*****************************************************************************/
class Input final : public Singletonbase<Input> 
{
	enum : int {KeyMax = 256};
private:
	//중복 키 허용을 위해 이전 키와 현재 키 변수 사용
	std::bitset<KeyMax> mKeyPast;
	std::bitset<KeyMax> mKeyCurrent;
	Vector2 mMousePosition;
public:
	Input();
	virtual ~Input();

	void Update();
	//한번 눌렸다면
	bool GetKeyDown(int key);
	//키에서 손을 땠다면
	bool GetKeyUp(int key);
	//키를 누르고 있다면
	bool GetKey(int key);
	//토글 키
	bool ToggleKey(int key);

	LRESULT InputProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	inline Vector2 GetMousePosition()const { return this->mMousePosition; }
	POINT GetMousePositionPoint();
};

#define _Input Input::Get()
