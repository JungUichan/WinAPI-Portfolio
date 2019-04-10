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
	//�� �߰�
	_SceneManager->AddScene("TestScene", new TestScene);

	//�ε� �� new ����
	LoadingScene* loadingScene = new LoadingScene;
	loadingScene->SetNextSceneName("TestScene");

	//�ε� �� �߰�
	_SceneManager->AddScene("LoadingScene", loadingScene);
	//�ε� �� �ҷ�����
	_SceneManager->LoadScene("LoadingScene");

	_SceneManager->SceneQueue();

	//�ҷ��� ���� Init()
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
	
	//���� ����
	_D2DRenderer->BeginRender();
	{
		_SceneManager->Render();

		if (_isDebug)
		{
			//�ð� �� ������ üũ
			_TimeManager->Render();
		}

	}
	// ���� ����
	_D2DRenderer->EndRender();
	_SceneManager->SceneQueue();
}
