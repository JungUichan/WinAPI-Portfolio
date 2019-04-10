#include "stdafx.h"
#include "SceneManager.h"
#include "SceneBase.h"

/**************************************************************
## SceneManager ## 생성자
***************************************************************/
SceneManager::SceneManager()
	:_nowScene(nullptr), _loadFunc(nullptr), _sceneState(SceneState::None), _fadeAlpha(1.f)
{
}

/**************************************************************
## SceneManager ## 소멸자
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
		
		//페이트 알파 값이 1.0f를 넘어가면
		if (_fadeAlpha >= 1.0f)
		{
			//페이트 알파 값  1.0f로 고정
			_fadeAlpha = 1.0f;
			//씬 상태는 None
			_sceneState = SceneState::None;
		}
		break;
	case SceneManager::SceneState::FadeIn:
		_fadeAlpha -= 1.0f * _TimeManager->DeltaTime();

		//페이트 알파 값이 0.f보다 낮아지면
		if (_fadeAlpha <= 0.f)
		{
			//페이트 알파 값  0.f로 고정
			_fadeAlpha = 0.f;
			//씬 상태는 None
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
	//현재 씬 렌더
	if (_nowScene != nullptr)
		_nowScene->Render();
	//만일 페이드 알파값과 0.f와 다르면
	if (FLOAT_EQUAL(_fadeAlpha, 0.f) == false)
		//검은색 채워진 렉트 생성
		_D2DRenderer->FillRectangle(Figure::FloatRect(Vector2(0,0),Vector2(_WinSizeX, _WinSizeY),Pivot::LeftTop), D2D1::ColorF::Black, _fadeAlpha, false);
}	

/**************************************************************
## SceneManager ## SceneQueue
***************************************************************/
void SceneManager::SceneQueue()
{
	//만일 씬 상태가 None이고
	if (_sceneState == SceneState::None)
	{
		//불러올 함수가 nullptr이 아니면
		if (_loadFunc != nullptr)
		{
			//불러올 씬 이름 키 값과 init bool
			_loadFunc(_loadSceneName, _isInit);
			_loadFunc = nullptr;
		}
	}
}
/**************************************************************
## SceneManager ## AddScene
: 씬 추가
***************************************************************/
void SceneManager::AddScene(string name, SceneBase * scene)
{
	SceneIter iter = _sceneList.find(name);
	if (iter != _sceneList.end()) return;
	this->_sceneList.insert(make_pair(name, scene));
}
/**************************************************************
## SceneManager ## FindScene
: 씬 찾기
***************************************************************/
SceneBase* SceneManager::FindScene(string name)
{
	SceneIter iter = _sceneList.find(name);
	if (iter != _sceneList.end()) return iter->second;

	return nullptr;
}
/**************************************************************
## SceneManager ## LoadScene
: 씬 불러오기
@@ 불러올 씬 키 값, 불러올 씬 init 여부 true/false
***************************************************************/
//## SceneManager ## ChangeScene
void SceneManager::ChangeScene(string name, bool init)
{
	SceneBase* findScene = FindScene(name);
	if (findScene != nullptr)
	{
		if (_nowScene)
			_nowScene->Release();

		//씬 변경
		_nowScene = findScene;

		if (init)
			_nowScene->Init();

		//씬 상태를 페이드 인으로
		this->_sceneState = SceneState::FadeIn;
	}
}

void SceneManager::LoadScene(string name, bool init)
{
	//bind : 함수 포인터에 함수를 담기 위해 사용
	//ChangeScene에 이름 키 값과 init 여부
	this->_loadFunc = bind(&SceneManager::ChangeScene, this, name, init);

	
	this->_loadSceneName = name;
	this->_isInit = init;
	//씬 상태를 페이드 아웃으로
	this->_sceneState = SceneState::FadeOut;
}

