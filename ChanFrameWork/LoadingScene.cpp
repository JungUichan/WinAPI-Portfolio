#include "stdafx.h"
#include "LoadingScene.h"
#include "Animation.h"
#include "Program.h"

/*************************************************
 ## LoadingScene ## ������
**************************************************/
LoadingScene::LoadingScene()
	:_loadingWString(L"Loading"), _frameCount(0.f)
{
	/**************************************************************
	�ε� �� �̹����� �̹��� �������� �ִٸ� ���⼭ �ε� �ҷ����ڸ��� ó��
	***************************************************************/
	//_loadingImage = _ImageManager->AddFrameImage("loading", PathResources(L"Scene/loading.png"), 34, 1);
	//_loadingAnimation = new Animation;
	//_loadingAnimation->SetStartEndFrame(0, 0, 33, 0, true);
	//_loadingAnimation->SetFrameUpdateTime(0.1f);
	//_loadingAnimation->SetIsLoop(true);
	//_loadingAnimation->Play();
}

/*************************************************
 ## LoadingScene ## �Ҹ���
**************************************************/
LoadingScene::~LoadingScene()
{
	//SafeDelete(_loadingAnimation);
}

void LoadingScene::Init()
{
	_loadingWString = L"Loading";

	//���ҽ��� ������ �ҷ�����
	this->LoadResource();
}

void LoadingScene::Release()
{
}

void LoadingScene::Update()
{
	//ȭ�鿡 �� �� �׸��� �ð�
	float deltaTime = _TimeManager->DeltaTime();

	//������ ī��Ʈ�� ȭ�鿡 �� �� �׸��� �ð� ��ŭ ����
	_frameCount += deltaTime;
	//������ �ð��� ȭ�鿡 �� �� �׸��� �ð� ��ŭ ����
	_delayTime += deltaTime;

	//���� ������ ī��Ʈ�� 1���� Ŀ���ų� ������
	if (_frameCount >= 1.0f)
	{
		//������ ī��Ʈ�� 1���� Ŀ���ų� ���� ���� 
		while (_frameCount >= 1.0f)
		{
			//������ ī��Ʈ 0.5�� ����
			_frameCount -= 0.5f;
		}
		//���� �ε� ���� ���̰� 12���� �۰ų� ������
		if (_loadingWString.length() <= 12)
		{
			//�ε� ������ . �߰��ϱ�
			_loadingWString += L".";
		}
		//�ƴ϶�� �ٽ� Loading ���� ���̱� (�ʱ�ȭ)
		else
		{
			_loadingWString = L"Loading";
		}
	}

	//@@ �ε� �� @@
	//�ҷ��� �� �̸�, init���� false
	//������ �ð��� 1���� Ŀ���ų� ������
	if (_delayTime >= 1.5f) 
	{
		//�ش� �� �ҷ�����
		_SceneManager->LoadScene(_nextSceneName, true);
	}

	//�ִϸ��̼� ������Ʈ ������
	//_loadingAnimation->UpdateFrame();
}

void LoadingScene::Render()
{
	//ȭ�� ������ ũ�⸸�� ��ĥ�� ��Ʈ �׸���
	//�ε� �� ��¿� ���� ��Ʈ
	_D2DRenderer->FillRectangle(Figure::FloatRect(Vector2(0, 0), Vector2(_WinSizeX, _WinSizeY),Pivot::LeftTop));

	//���� �׸���
	_D2DRenderer->RenderTextField(_WinSizeX / 2 - _loadingWString.size() / 2 * 10, _WinSizeY / 2 + 200,
		_loadingWString, 30, 300, 100, D2DRenderer::DefaultBrush::White, DWRITE_TEXT_ALIGNMENT_JUSTIFIED);

	//_loadingImage->SetSize(_loadingImage->GetFrameSize(0));
	//_loadingImage->SetScale(1.3f);
	//_loadingImage->FrameRender(929, 590, _loadingAnimation->GetNowFrameX(), 0, Pivot::Center, false);
}
