#include "stdafx.h"
#include "SceneManager.h"
#include "SceneBase.h"

/**************************************************************
## SceneManager ## ������
***************************************************************/
SceneManager::SceneManager()
	:_nowScene(nullptr), _loadFunc(nullptr), _sceneState(SceneState::None), _fadeAlpha(1.f)
{
}

/**************************************************************
## SceneManager ## �Ҹ���
***************************************************************/
SceneManager::~SceneManager()
{
	SceneIter iter = _sceneList.begin();
	for (; iter != _sceneList.end();) 
	{
		if (iter->second != nullptr) 
		{
			if (iter->second == _nowScene)iter->second->Release();
			SafeDelete(iter->second);
			iter = _sceneList.erase(iter);
		}
		else 
		{
			++iter;
		}
	}
	
	_sceneList.clear();
}

/**************************************************************
## SceneManager ## Update
***************************************************************/
void SceneManager::Update()
{
	if (_nowScene) 
	{
		_nowScene->Update();
	}

	switch (_sceneState)
	{
	case SceneManager::SceneState::None:
		break;
	case SceneManager::SceneState::FadeOut:
		_fadeAlpha += 1.0f * _TimeManager->DeltaTime();
		
		//����Ʈ ���� ���� 1.0f�� �Ѿ��
		if (_fadeAlpha >= 1.0f)
		{
			//����Ʈ ���� ��  1.0f�� ����
			_fadeAlpha = 1.0f;
			//�� ���´� None
			_sceneState = SceneState::None;
		}
		break;
	case SceneManager::SceneState::FadeIn:
		_fadeAlpha -= 1.0f * _TimeManager->DeltaTime();

		//����Ʈ ���� ���� 0.f���� ��������
		if (_fadeAlpha <= 0.f)
		{
			//����Ʈ ���� ��  0.f�� ����
			_fadeAlpha = 0.f;
			//�� ���´� None
			_sceneState = SceneState::None;
		}
		break;
	case SceneManager::SceneState::Load:
		break;
	case SceneManager::SceneState::End:
		break;
	default:
		break;
	}

}
/**************************************************************
## SceneManager ## Render
***************************************************************/
void SceneManager::Render()
{
	//���� �� ����
	if (_nowScene != nullptr)
		_nowScene->Render();
	//���� ���̵� ���İ��� 0.f�� �ٸ���
	if (FLOAT_EQUAL(_fadeAlpha, 0.f) == false)
		//������ ä���� ��Ʈ ����
		_D2DRenderer->FillRectangle(Figure::FloatRect(Vector2(0,0),Vector2(_WinSizeX, _WinSizeY),Pivot::LeftTop), D2D1::ColorF::Black, _fadeAlpha, false);
}	

/**************************************************************
## SceneManager ## SceneQueue
***************************************************************/
void SceneManager::SceneQueue()
{
	//���� �� ���°� None�̰�
	if (_sceneState == SceneState::None)
	{
		//�ҷ��� �Լ��� nullptr�� �ƴϸ�
		if (_loadFunc != nullptr)
		{
			//�ҷ��� �� �̸� Ű ���� init bool
			_loadFunc(_loadSceneName, _isInit);
			_loadFunc = nullptr;
		}
	}
}
/**************************************************************
## SceneManager ## AddScene
: �� �߰�
***************************************************************/
void SceneManager::AddScene(string name, SceneBase * scene)
{
	SceneIter iter = _sceneList.find(name);
	if (iter != _sceneList.end()) return;
	this->_sceneList.insert(make_pair(name, scene));
}
/**************************************************************
## SceneManager ## FindScene
: �� ã��
***************************************************************/
SceneBase* SceneManager::FindScene(string name)
{
	SceneIter iter = _sceneList.find(name);
	if (iter != _sceneList.end()) return iter->second;

	return nullptr;
}
/**************************************************************
## SceneManager ## LoadScene
: �� �ҷ�����
@@ �ҷ��� �� Ű ��, �ҷ��� �� init ���� true/false
***************************************************************/
//## SceneManager ## ChangeScene
void SceneManager::ChangeScene(string name, bool init)
{
	SceneBase* findScene = FindScene(name);
	if (findScene != nullptr)
	{
		if (_nowScene)
			_nowScene->Release();

		//�� ����
		_nowScene = findScene;

		if (init)
			_nowScene->Init();

		//�� ���¸� ���̵� ������
		this->_sceneState = SceneState::FadeIn;
	}
}

void SceneManager::LoadScene(string name, bool init)
{
	//bind : �Լ� �����Ϳ� �Լ��� ��� ���� ���
	//ChangeScene�� �̸� Ű ���� init ����
	this->_loadFunc = bind(&SceneManager::ChangeScene, this, name, init);

	
	this->_loadSceneName = name;
	this->_isInit = init;
	//�� ���¸� ���̵� �ƿ�����
	this->_sceneState = SceneState::FadeOut;
}

