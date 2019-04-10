#pragma once
#include <functional>
/**********************************************
## Animation ##
이미지의 프레임을 담당하는 클래스
: 프레임을 돌리는 기능 담당
***********************************************/
class Animation
{
public:
	//프레임 돌릴 방향
	enum class FrameWay 
	{
		Horizontal = 0,					//가로
		Vertical = 1,					//세로
	};
private:
	bool _isPlay;						//프레임 작동
	bool _isLoop;						//프레임 반복 루프
	bool _isEnd;						//프레임 작동 종료

	int _currentFrameIndex;				//현재 프레임 인덱스
	
	float _currentFrameTime;			//현재 프레임을 재는 시간
	float _frameUpdateTime;				//프레임을 카운트를 증가시킬 때까지 걸리는 프레임 업데이트 시간
	
	vector<pair<int, int>>_frameList;	//프레임 정보를 담아둘 벡터
	function<void(void)> _func;			//프레임 다 감기면 실행 시킬 함수
										//function<void(void)>는 반환형 void 인자도 void인 함수를 받을 것이다.

public:
	Animation();
	~Animation();

	//_isEnd 받기
	bool GetIsEnd() { return _isEnd; }
	//프레임 업데이트
	bool UpdateFrame();

/**********************************************
## 프레임 기능 함수 ##
***********************************************/
	//프레임 실행
	void Play();
	//프레임 정지
	void Stop();
	//프레임 일시정지
	void Pause();

/**********************************************
## 프레임 정보 ##
***********************************************/
	//프레임 정보 받기
	pair<int, int> GetNowFrameData()const;
	//프레임 정보가 담긴 벡터 세팅
	void SetVectorFrame(vector<pair<int, int>> playFrame);

/**********************************************
## 프레임 시작과 끝 ##
***********************************************/
	//시작, 끝 프레임 세팅
	void SetStartEndFrame(int startX, int startY, int endX, int endY,
		bool reverse, FrameWay way = FrameWay::Horizontal);

/**********************************************
## 프레임 끝나면 함수 불러오기 ##
***********************************************/
	//프레임이 끝남과 동시에 불러울 함수 세팅
	void SetCallbackFunc(function<void(void)> func);

/**********************************************
## 프레임 루프 ##
***********************************************/
	//프레임 루프 상태 true/false 받기
	bool GetIsLoop()const { return this->_isLoop; }
	//프레임 루프 상태 true/false 세팅
	void SetIsLoop(bool loop) { this->_isLoop = loop; }

/**********************************************
## 프레임 업데이트 ##
***********************************************/
	//프레임 카운트 증가시킬 때까지 걸리는 프레임 업데이트 함수 받기
	float GetFrameUpdateTime()const { return this->_frameUpdateTime; }
	//프레임 카운트 증가시킬 때까지 걸리는 프레임 업데이트 함수 세팅
	void SetFrameUpdateTime(float FrameRate) { this->_frameUpdateTime = FrameRate; }

/**********************************************
## 현재 프레임 ##
***********************************************/
	//현재 X프레임 받기
	int GetNowFrameX()const;
	//현재 Y프레임 받기
	int GetNowFrameY()const;
	//현재 프레임 함수 세팅
	void SetCurrentFrame(int index) { _currentFrameIndex = index; }

};

