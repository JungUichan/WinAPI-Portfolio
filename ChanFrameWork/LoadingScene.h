#pragma once
#include "SceneBase.h"
class LoadingScene : public SceneBase
{
private:
	wstring  _loadingWString;			//로딩 문구
	float _frameCount;					//프레임 카운트
	float _delayTime;					//딜레이 시간
	class Image* _loadingImage;			//로딩 이미지
	class Animation* _loadingAnimation;	//로딩 이미지 프레임 돌릴 애니메이션
	string _nextSceneName;				//다음 씬 이름
public:
	LoadingScene();
	~LoadingScene();

	void Init()override;
	void Release()override;
	void Update()override;
	void Render()override;

	void SetNextSceneName(const string name) { _nextSceneName = name; }

	//해당 로드 리소스 함수는 SceneInit에서 관리
	void LoadResource();
};

