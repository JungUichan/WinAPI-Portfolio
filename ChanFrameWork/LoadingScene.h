#pragma once
#include "SceneBase.h"
class LoadingScene : public SceneBase
{
private:
	wstring  _loadingWString;			//�ε� ����
	float _frameCount;					//������ ī��Ʈ
	float _delayTime;					//������ �ð�
	class Image* _loadingImage;			//�ε� �̹���
	class Animation* _loadingAnimation;	//�ε� �̹��� ������ ���� �ִϸ��̼�
	string _nextSceneName;				//���� �� �̸�
public:
	LoadingScene();
	~LoadingScene();

	void Init()override;
	void Release()override;
	void Update()override;
	void Render()override;

	void SetNextSceneName(const string name) { _nextSceneName = name; }

	//�ش� �ε� ���ҽ� �Լ��� SceneInit���� ����
	void LoadResource();
};

