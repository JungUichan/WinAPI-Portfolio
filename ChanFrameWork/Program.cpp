#include "stdafx.h"
#include "Program.h"
#include "GameObject.h"

#include "TestScene.h"
#include "LoadingScene.h"

Program::Program()
{
}

Program::~Program()
{
}

void Program::Init()
{
	//씬 추가
	_SceneManager->AddScene("TestScene", new TestScene);

	//로딩 씬 new 선언
	LoadingScene* loadingScene = new LoadingScene;
	loadingScene->SetNextSceneName("TestScene");

	//로딩 씬 추가
	_SceneManager->AddScene("LoadingScene", loadingScene);
	//로딩 씬 불러오기
	_SceneManager->LoadScene("LoadingScene");

	_SceneManager->SceneQueue();

	//불러온 씬의 Init()
	//_SceneManager->FindScene("TestScene")->Init();

	_CameraManager->SetCameraState(Camera::CameraState::End);
}

void Program::Release()
{
}

void Program::Update()
{
	if (_Input->GetKeyDown(VK_F1))
		_isDebug = !_isDebug;


	_SceneManager->Update();
}

void Program::Render()
{
	
	//렌더 시작
	_D2DRenderer->BeginRender();
	{
		_SceneManager->Render();

		if (_isDebug)
		{
			//시간 및 프레임 체크
			_TimeManager->Render();
		}

	}
	// 렌더 종료
	_D2DRenderer->EndRender();
	_SceneManager->SceneQueue();
}
