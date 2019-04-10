#include "stdafx.h"
#include "Animation.h"

/*********************************************************
## Animation ## 생성자
**********************************************************/
Animation::Animation()
	//멤버 이니셜라이즈 인자 초기화
	:_isPlay(false), _isLoop(true), _currentFrameIndex(0),
	_currentFrameTime(0.f), _frameUpdateTime(0.3f), _func(nullptr), _isEnd(false)
{
}

/*********************************************************
## Animation ## 소멸자
**********************************************************/
Animation::~Animation()
{
	//벡터에 담긴 정보를 클리어
	this->_frameList.clear();
}

bool Animation::UpdateFrame()
{
	//프레임 작동 종료 false
	_isEnd = false;

	//만약 플레이 상태가 true라면
	if (this->_isPlay == true)
	{
		//현재 프레임 감기까지 걸리는 시간은 델타 타임(화면에 한 번 그리는 시간)에 따라 증가 
		this->_currentFrameTime += _TimeManager->DeltaTime();
		//만약 현재 프레임 감기까지 걸리는 시간이 프레임 감기까지 걸리는 프레임 업데이트 시간보다 커졌다면 
		if (_currentFrameTime >= _frameUpdateTime)
		{
			//프레임 업데이트 시간보다 커진동안 
			while (_currentFrameTime >= _frameUpdateTime)
			{
				//프레임 감는 시간보다 넘어간 시간만큼 깎아 주기
				_currentFrameTime -= _frameUpdateTime;
			}
			//프레임 인덱스 증가
			++_currentFrameIndex;
			//프레임 인덱스가 프레임 데이터 사이즈보다 커질려고 한다면
			if (_currentFrameIndex >= CastingInt(_frameList.size()))
			{
				//만약 루프 상태가 false면 
				if (_isLoop == false)
				{
					//플레이 상태 false
					_isPlay = false;
					//현재 프레임 인덱스 감소
					--_currentFrameIndex;
				}
				//루프 상태가 false가 아닌 경우엔
				else
				{
					//현재 프레임 인덱스는 0으로 초기화
					this->_currentFrameIndex = 0;
				}
				//만약 실행시킬 함수가 있다면 실행시켜라 
				//람다식 함수 호출을 위한 함수 값
				if (_func != nullptr)
					_func();
				//프레임 작동 정지 상태 true처리
				_isEnd = true;
				
				//true 반환 후 빠져나오기
				return true;
			}
		}
	}
	//false 반환 후 빠져나오기
	return false;
}

/**************************************************
## Play ## 실행
***************************************************/
void Animation::Play()
{
	this->_isPlay = true;
}

/**************************************************
## Stop ## 정지
***************************************************/
void Animation::Stop()
{
	this->_isPlay = false;
	//애니메이션이 정지 후 프레임과 시간 값 0으로 초기화
	this->_currentFrameIndex = 0;
	this->_currentFrameTime = 0.f;
}

/**************************************************
## Pause ## 일시 정지
***************************************************/
void Animation::Pause()
{
	this->_isPlay = false;
}


/**************************************************
## GetNowFrameData ##
현재 프레임 데이터 반환
***************************************************/
pair<int, int> Animation::GetNowFrameData() const
{
	return this->_frameList[_currentFrameIndex];
}

/************************************************************
## SetVectorFrame ##
기존 프레임 리스트 비우고, 새로운 프레임 정보를 담은 벡터 프레임을 세팅
*************************************************************/
void Animation::SetVectorFrame(vector<pair<int, int>> playFrame)
{
	//프레임 리스트 비우기(클리어)
	this->_frameList.clear();
	//프레임 리스트에 인자 프레임 정보 담기
	this->_frameList = playFrame;
}

/************************************************************
## SetVectorFrame ##
@@ startX, Y : 시작 인덱스 X, Y
@@ endX, Y : 끝 인덱스 X, Y
@@ reverse : 프레임 반전
@@ FrameWay : 프레임 방향 (가로, 세로)
*************************************************************/
void Animation::SetStartEndFrame(int startX, int startY, int endX, int endY, bool reverse, FrameWay way)
{
	//프레임 리스트 비우기(클리어)
	this->_frameList.clear();

	//만일 프레임 방향이 가로라면
	if (way == FrameWay::Horizontal) 
	{
		//시작 Y부터 끝 Y프레임까지 for문
		for (int y = startY; y <= endY; ++y) 
		{
			//시작 X부터 끝 X프레임까지 for문
			for (int x = startX; x <= endX; ++x) 
			{
				//프레임 리스트에 x, y 프레임 정보 담기
				this->_frameList.push_back(make_pair(x, y));
			}
		}
		//만일 reverse가 true라면(=반전 상태라면)
		if (reverse == true) 
		{
			//시작 Y부터 끝 Y프레임까지 for문
			for (int y = startY; y <= endY; ++y)
			{
				//시작 X부터 끝 X프레임까지 for문
				for (int x = startX; x <= endX; ++x)
				{
					//프레임 리스트에 x, y 프레임 정보 담기
					this->_frameList.push_back(make_pair(x, y));
				}
			}
		}
	}
	//만일 프레임 방향이 세로라면
	else if (way == FrameWay::Vertical) 
	{
		//시작 Y부터 끝 Y프레임까지 for문
		for (int y = startY; y <= endY; ++y)
		{
			//시작 X부터 끝 X프레임까지 for문
			for (int x = startX; x <= endX; ++x)
			{
				//프레임 리스트에 x, y 프레임 정보 담기
				this->_frameList.push_back(make_pair(x, y));
			}
		}
		//만일 reverse가 true라면(=반전 상태라면)
		if (reverse == true)
		{
			//시작 Y부터 끝 Y프레임까지 for문
			for (int y = startY; y <= endY; ++y)
			{
				//시작 X부터 끝 X프레임까지 for문
				for (int x = startX; x <= endX; ++x)
				{
					//프레임 리스트에 x, y 프레임 정보 담기
					this->_frameList.push_back(make_pair(x, y));
				}
			}
		}
	}
}

/*************************************************************
## SetCallbackFunc ##
@@ function<void()> func : 애니메이션 끝날 때 실행할 콜백함수
**************************************************************/
void Animation::SetCallbackFunc(function<void(void)> func)
{
	this->_func = func;
}
/*************************************************************
## GetNowFrameX ##
현재 프레임 X
**************************************************************/
int Animation::GetNowFrameX() const
{
	//int X
	return this->_frameList[_currentFrameIndex].first;
}

/*************************************************************
## GetNowFrameY ##
현재 프레임 Y
**************************************************************/
int Animation::GetNowFrameY() const
{
	//int Y
	return this->_frameList[_currentFrameIndex].second;
}
