#pragma once
#include <functional>
/**********************************************
## Animation ##
�̹����� �������� ����ϴ� Ŭ����
: �������� ������ ��� ���
***********************************************/
class Animation
{
public:
	//������ ���� ����
	enum class FrameWay 
	{
		Horizontal = 0,					//����
		Vertical = 1,					//����
	};
private:
	bool _isPlay;						//������ �۵�
	bool _isLoop;						//������ �ݺ� ����
	bool _isEnd;						//������ �۵� ����

	int _currentFrameIndex;				//���� ������ �ε���
	
	float _currentFrameTime;			//���� �������� ��� �ð�
	float _frameUpdateTime;				//�������� ī��Ʈ�� ������ų ������ �ɸ��� ������ ������Ʈ �ð�
	
	vector<pair<int, int>>_frameList;	//������ ������ ��Ƶ� ����
	function<void(void)> _func;			//������ �� ����� ���� ��ų �Լ�
										//function<void(void)>�� ��ȯ�� void ���ڵ� void�� �Լ��� ���� ���̴�.

public:
	Animation();
	~Animation();

	//_isEnd �ޱ�
	bool GetIsEnd() { return _isEnd; }
	//������ ������Ʈ
	bool UpdateFrame();

/**********************************************
## ������ ��� �Լ� ##
***********************************************/
	//������ ����
	void Play();
	//������ ����
	void Stop();
	//������ �Ͻ�����
	void Pause();

/**********************************************
## ������ ���� ##
***********************************************/
	//������ ���� �ޱ�
	pair<int, int> GetNowFrameData()const;
	//������ ������ ��� ���� ����
	void SetVectorFrame(vector<pair<int, int>> playFrame);

/**********************************************
## ������ ���۰� �� ##
***********************************************/
	//����, �� ������ ����
	void SetStartEndFrame(int startX, int startY, int endX, int endY,
		bool reverse, FrameWay way = FrameWay::Horizontal);

/**********************************************
## ������ ������ �Լ� �ҷ����� ##
***********************************************/
	//�������� ������ ���ÿ� �ҷ��� �Լ� ����
	void SetCallbackFunc(function<void(void)> func);

/**********************************************
## ������ ���� ##
***********************************************/
	//������ ���� ���� true/false �ޱ�
	bool GetIsLoop()const { return this->_isLoop; }
	//������ ���� ���� true/false ����
	void SetIsLoop(bool loop) { this->_isLoop = loop; }

/**********************************************
## ������ ������Ʈ ##
***********************************************/
	//������ ī��Ʈ ������ų ������ �ɸ��� ������ ������Ʈ �Լ� �ޱ�
	float GetFrameUpdateTime()const { return this->_frameUpdateTime; }
	//������ ī��Ʈ ������ų ������ �ɸ��� ������ ������Ʈ �Լ� ����
	void SetFrameUpdateTime(float FrameRate) { this->_frameUpdateTime = FrameRate; }

/**********************************************
## ���� ������ ##
***********************************************/
	//���� X������ �ޱ�
	int GetNowFrameX()const;
	//���� Y������ �ޱ�
	int GetNowFrameY()const;
	//���� ������ �Լ� ����
	void SetCurrentFrame(int index) { _currentFrameIndex = index; }

};

