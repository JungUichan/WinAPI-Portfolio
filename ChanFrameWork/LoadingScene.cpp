#include "stdafx.h"
#include "LoadingScene.h"
#include "Animation.h"
#include "Program.h"

/*************************************************
 ## LoadingScene ## 생성자
**************************************************/
LoadingScene::LoadingScene()
	:_loadingWString(L"Loading"), _frameCount(0.f)
{
	/**************************************************************
	로딩 씬 이미지와 이미지 프레임이 있다면 여기서 로딩 불러오자마자 처리
	***************************************************************/
	//_loadingImage = _ImageManager->AddFrameImage("loading", PathResources(L"Scene/loading.png"), 34, 1);
	//_loadingAnimation = new Animation;
	//_loadingAnimation->SetStartEndFrame(0, 0, 33, 0, true);
	//_loadingAnimation->SetFrameUpdateTime(0.1f);
	//_loadingAnimation->SetIsLoop(true);
	//_loadingAnimation->Play();
}

/*************************************************
 ## LoadingScene ## 소멸자
**************************************************/
LoadingScene::~LoadingScene()
{
	//SafeDelete(_loadingAnimation);
}

void LoadingScene::Init()
{
	_loadingWString = L"Loading";

	//리소스와 데이터 불러오기
	this->LoadResource();
}

void LoadingScene::Release()
{
}

void LoadingScene::Update()
{
	//화면에 한 번 그리는 시간
	float deltaTime = _TimeManager->DeltaTime();

	//프레임 카운트는 화면에 한 번 그리는 시간 만큼 증가
	_frameCount += deltaTime;
	//딜레이 시간은 화면에 한 번 그리는 시간 만큼 증가
	_delayTime += deltaTime;

	//만일 프레임 카운트가 1보다 커지거나 같으면
	if (_frameCount >= 1.0f)
	{
		//프레임 카운트가 1보다 커지거나 같은 동안 
		while (_frameCount >= 1.0f)
		{
			//프레임 카운트 0.5씩 감소
			_frameCount -= 0.5f;
		}
		//만일 로딩 문구 길이가 12보다 작거나 같으면
		if (_loadingWString.length() <= 12)
		{
			//로딩 문구에 . 추가하기
			_loadingWString += L".";
		}
		//아니라면 다시 Loading 문구 보이기 (초기화)
		else
		{
			_loadingWString = L"Loading";
		}
	}

	//@@ 로드 씬 @@
	//불러올 씬 이름, init여부 false
	//딜레이 시간이 1보다 커지거나 같으면
	if (_delayTime >= 1.5f) 
	{
		//해당 씬 불러오기
		_SceneManager->LoadScene(_nextSceneName, true);
	}

	//애니메이션 업데이트 프레임
	//_loadingAnimation->UpdateFrame();
}

void LoadingScene::Render()
{
	//화면 사이즈 크기만한 색칠된 렉트 그리기
	//로딩 씬 출력용 연출 렉트
	_D2DRenderer->FillRectangle(Figure::FloatRect(Vector2(0, 0), Vector2(_WinSizeX, _WinSizeY),Pivot::LeftTop));

	//문자 그리기
	_D2DRenderer->RenderTextField(_WinSizeX / 2 - _loadingWString.size() / 2 * 10, _WinSizeY / 2 + 200,
		_loadingWString, 30, 300, 100, D2DRenderer::DefaultBrush::White, DWRITE_TEXT_ALIGNMENT_JUSTIFIED);

	//_loadingImage->SetSize(_loadingImage->GetFrameSize(0));
	//_loadingImage->SetScale(1.3f);
	//_loadingImage->FrameRender(929, 590, _loadingAnimation->GetNowFrameX(), 0, Pivot::Center, false);
}
