#include "stdafx.h"
#include "Animation.h"

/*********************************************************
## Animation ## ������
**********************************************************/
Animation::Animation()
	//��� �̴ϼȶ����� ���� �ʱ�ȭ
	:_isPlay(false), _isLoop(true), _currentFrameIndex(0),
	_currentFrameTime(0.f), _frameUpdateTime(0.3f), _func(nullptr), _isEnd(false)
{
}

/*********************************************************
## Animation ## �Ҹ���
**********************************************************/
Animation::~Animation()
{
	//���Ϳ� ��� ������ Ŭ����
	this->_frameList.clear();
}

bool Animation::UpdateFrame()
{
	//������ �۵� ���� false
	_isEnd = false;

	//���� �÷��� ���°� true���
	if (this->_isPlay == true)
	{
		//���� ������ ������� �ɸ��� �ð��� ��Ÿ Ÿ��(ȭ�鿡 �� �� �׸��� �ð�)�� ���� ���� 
		this->_currentFrameTime += _TimeManager->DeltaTime();
		//���� ���� ������ ������� �ɸ��� �ð��� ������ ������� �ɸ��� ������ ������Ʈ �ð����� Ŀ���ٸ� 
		if (_currentFrameTime >= _frameUpdateTime)
		{
			//������ ������Ʈ �ð����� Ŀ������ 
			while (_currentFrameTime >= _frameUpdateTime)
			{
				//������ ���� �ð����� �Ѿ �ð���ŭ ��� �ֱ�
				_currentFrameTime -= _frameUpdateTime;
			}
			//������ �ε��� ����
			++_currentFrameIndex;
			//������ �ε����� ������ ������ ������� Ŀ������ �Ѵٸ�
			if (_currentFrameIndex >= CastingInt(_frameList.size()))
			{
				//���� ���� ���°� false�� 
				if (_isLoop == false)
				{
					//�÷��� ���� false
					_isPlay = false;
					//���� ������ �ε��� ����
					--_currentFrameIndex;
				}
				//���� ���°� false�� �ƴ� ��쿣
				else
				{
					//���� ������ �ε����� 0���� �ʱ�ȭ
					this->_currentFrameIndex = 0;
				}
				//���� �����ų �Լ��� �ִٸ� ������Ѷ� 
				//���ٽ� �Լ� ȣ���� ���� �Լ� ��
				if (_func != nullptr)
					_func();
				//������ �۵� ���� ���� trueó��
				_isEnd = true;
				
				//true ��ȯ �� ����������
				return true;
			}
		}
	}
	//false ��ȯ �� ����������
	return false;
}

/**************************************************
## Play ## ����
***************************************************/
void Animation::Play()
{
	this->_isPlay = true;
}

/**************************************************
## Stop ## ����
***************************************************/
void Animation::Stop()
{
	this->_isPlay = false;
	//�ִϸ��̼��� ���� �� �����Ӱ� �ð� �� 0���� �ʱ�ȭ
	this->_currentFrameIndex = 0;
	this->_currentFrameTime = 0.f;
}

/**************************************************
## Pause ## �Ͻ� ����
***************************************************/
void Animation::Pause()
{
	this->_isPlay = false;
}


/**************************************************
## GetNowFrameData ##
���� ������ ������ ��ȯ
***************************************************/
pair<int, int> Animation::GetNowFrameData() const
{
	return this->_frameList[_currentFrameIndex];
}

/************************************************************
## SetVectorFrame ##
���� ������ ����Ʈ ����, ���ο� ������ ������ ���� ���� �������� ����
*************************************************************/
void Animation::SetVectorFrame(vector<pair<int, int>> playFrame)
{
	//������ ����Ʈ ����(Ŭ����)
	this->_frameList.clear();
	//������ ����Ʈ�� ���� ������ ���� ���
	this->_frameList = playFrame;
}

/************************************************************
## SetVectorFrame ##
@@ startX, Y : ���� �ε��� X, Y
@@ endX, Y : �� �ε��� X, Y
@@ reverse : ������ ����
@@ FrameWay : ������ ���� (����, ����)
*************************************************************/
void Animation::SetStartEndFrame(int startX, int startY, int endX, int endY, bool reverse, FrameWay way)
{
	//������ ����Ʈ ����(Ŭ����)
	this->_frameList.clear();

	//���� ������ ������ ���ζ��
	if (way == FrameWay::Horizontal) 
	{
		//���� Y���� �� Y�����ӱ��� for��
		for (int y = startY; y <= endY; ++y) 
		{
			//���� X���� �� X�����ӱ��� for��
			for (int x = startX; x <= endX; ++x) 
			{
				//������ ����Ʈ�� x, y ������ ���� ���
				this->_frameList.push_back(make_pair(x, y));
			}
		}
		//���� reverse�� true���(=���� ���¶��)
		if (reverse == true) 
		{
			//���� Y���� �� Y�����ӱ��� for��
			for (int y = startY; y <= endY; ++y)
			{
				//���� X���� �� X�����ӱ��� for��
				for (int x = startX; x <= endX; ++x)
				{
					//������ ����Ʈ�� x, y ������ ���� ���
					this->_frameList.push_back(make_pair(x, y));
				}
			}
		}
	}
	//���� ������ ������ ���ζ��
	else if (way == FrameWay::Vertical) 
	{
		//���� Y���� �� Y�����ӱ��� for��
		for (int y = startY; y <= endY; ++y)
		{
			//���� X���� �� X�����ӱ��� for��
			for (int x = startX; x <= endX; ++x)
			{
				//������ ����Ʈ�� x, y ������ ���� ���
				this->_frameList.push_back(make_pair(x, y));
			}
		}
		//���� reverse�� true���(=���� ���¶��)
		if (reverse == true)
		{
			//���� Y���� �� Y�����ӱ��� for��
			for (int y = startY; y <= endY; ++y)
			{
				//���� X���� �� X�����ӱ��� for��
				for (int x = startX; x <= endX; ++x)
				{
					//������ ����Ʈ�� x, y ������ ���� ���
					this->_frameList.push_back(make_pair(x, y));
				}
			}
		}
	}
}

/*************************************************************
## SetCallbackFunc ##
@@ function<void()> func : �ִϸ��̼� ���� �� ������ �ݹ��Լ�
**************************************************************/
void Animation::SetCallbackFunc(function<void(void)> func)
{
	this->_func = func;
}
/*************************************************************
## GetNowFrameX ##
���� ������ X
**************************************************************/
int Animation::GetNowFrameX() const
{
	//int X
	return this->_frameList[_currentFrameIndex].first;
}

/*************************************************************
## GetNowFrameY ##
���� ������ Y
**************************************************************/
int Animation::GetNowFrameY() const
{
	//int Y
	return this->_frameList[_currentFrameIndex].second;
}
