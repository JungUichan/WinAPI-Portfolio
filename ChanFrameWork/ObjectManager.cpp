#include "stdafx.h"
#include "ObjectManager.h"
#include "GameObject.h"
#include <algorithm>
#include <unordered_map>

/*********************************************************************
## ObjectManager ##
-> 생성자 
**********************************************************************/
ObjectManager::ObjectManager()
	:_isZorder(false)
{
	for (UINT i = 0; i < ObjectType::END; ++i) 
	{
		_objectContainer.insert(make_pair((ObjectType::Enum)i, vector<GameObject*>()));
	}
}

/*********************************************************************
## ~ObjectManager ##
-> 소멸자, 모든 오브젝트 메모리 해제 및 컨테이너 클리어
**********************************************************************/
ObjectManager::~ObjectManager()
{
	//안정성을 위해 소멸자에서 한번 더 Release
	ObjectContainerIter iter = _objectContainer.begin();
	for (; iter != _objectContainer.end(); ++iter)
	{
		for (UINT i = 0; i < iter->second.size(); ++i) 
		{
			iter->second[i]->Release();
			SafeDelete(iter->second[i]);
		}
		iter->second.clear();
	}
	_objectContainer.clear();
}

/************************************************************************************
## Init ##
*************************************************************************************/
void ObjectManager::Init()
{
	ObjectContainerIter iter = _objectContainer.begin();
	for (; iter != _objectContainer.end(); ++iter) 
	{
		for (UINT i = 0; i < iter->second.size(); ++i) 
		{
			iter->second[i]->Init();
		}
	}
}
/************************************************************************************
## Release ##
*************************************************************************************/
void ObjectManager::Release()	
{
	ObjectContainerIter iter = _objectContainer.begin();
	for (; iter != _objectContainer.end(); ++iter) 
	{
		for (UINT i = 0; i < iter->second.size(); ++i) 
		{
			iter->second[i]->Release();
			SafeDelete(iter->second[i]);
		}
		iter->second.clear();
	}
}

/************************************************************************************
## Update ##
*************************************************************************************/
void ObjectManager::Update()
{
	ObjectContainerIter iter = _objectContainer.begin();
	for (; iter != _objectContainer.end(); ++iter) 
	{
		for (UINT i = 0; i < iter->second.size(); ++i) 
		{
			//만일 GetLive가 false라면
			if (iter->second[i]->GetLive() == false) 
			{
				//해당 인덱스를 Release시키고
				iter->second[i]->Release();
				//해당 인덱스를 SafeDelete시키고
				SafeDelete(iter->second[i]);
				//해당 인덱스를 erase로 지운다
				//메모리 해제 후 벡터 내 null 오브젝트 개체 제거
				//터지는 걸 방지하기 위해 끝에 i-- 
				iter->second.erase(iter->second.begin() + i--);
				continue;
			}
			//만일 GetActive가 true라면
			if (iter->second[i]->GetActive()) 
			{
				//해당 인덱스 Update 진행
				iter->second[i]->Update();
			}
		}
	}
}
/************************************************************************************
## Render ##
*************************************************************************************/
void ObjectManager::Render()
{
	//오브젝트 렌더
	this->ObjectRender();
	//UI 렌더
	this->UIRender();
}

/************************************************************************************
## AddObject ##
-> 오브젝트 등록 함수
: 오브젝트 타입과 오브젝트 2가지 인자를 받아서 등록
*************************************************************************************/
void ObjectManager::AddObject(const ObjectType::Enum objType, GameObject* pGameObject)
{
	this->_objectContainer[objType].push_back(pGameObject);
}

/************************************************************************************
## FindObject ##
-> 등록한 오브젝트 찾아서 가져오는 함수
: 오브젝트 타입과 해당 오브젝트 키 값 2가지 인자를 받아서 등록
*************************************************************************************/
GameObject* ObjectManager::FindObject(const ObjectType::Enum objType, const string keyName)
{
	//오브젝트 타입을 찾아서 가져오기
	ObjectContainerIter iter = _objectContainer.find(objType);
	for (UINT i = 0; i < iter->second.size(); ++i) 
	{
		//## 가져온 타입의 인덱스 탐색 ##
		//만일 해당 인덱스가 입력한 키 값이 같다면
		if (iter->second[i]->GetName() == keyName) 
		{
			//해당 인덱스 반환
			return iter->second[i];
		}
	}
	return nullptr;
}
/************************************************************************************
## FindObjects ##
-> 등록한 오브젝트들을 찾아서 가져오는 함수
: 오브젝트 타입과 해당 오브젝트 키 값 2가지 인자를 받아서 등록
*************************************************************************************/
vector<class GameObject*> ObjectManager::FindObjects(const ObjectType::Enum objType, const string keyName)
{
	//오브젝트 타입을 찾아서 가져오기
	vector<GameObject*> findList;
	ObjectContainerIter iter = _objectContainer.find(objType);
	for (UINT i = 0; i < iter->second.size(); ++i) 
	{
		//## 가져온 타입의 인덱스 탐색 ##
		//만일 해당 인덱스가 입력한 키 값이 같다면
		if (iter->second[i]->GetName() == keyName) 
		{
			//findList벡터에 탐색해서 찾은 인덱스들을 push_back으로 담기
			findList.push_back(iter->second[i]);
		}
	}
	//찾아서 등록한 findList를 반환
	return findList;
}
/************************************************************************************
## GetObjectList ##
-> 등록한 오브젝트 타입을 전부 찾아서 가져오는 함수
: 오브젝트 타입으로 인자를 받아서 등록
*************************************************************************************/
vector<class GameObject*> ObjectManager::GetObjectList(const ObjectType::Enum objType)
{
	return _objectContainer[objType];
}

/************************************************************************************
## GetObjectListPointer ##
-> 등록한 오브젝트 타입을 전부 찾아서 벡터 포인터로 가져오는 함수
: 오브젝트 타입으로 인자를 받아서 등록
: 벡터 연산으로 인해 생기는 연산량을 포인터로 접근해 주소만 가져와 프레임이 떨어지는 현상 방지
: const를 붙인 이유는 외부에서 접근하여 바꿀 우려가 있기에 안정성을 위해 사용 
*************************************************************************************/
const vector<class GameObject*>* ObjectManager::GetObjectListPointer(const ObjectType::Enum objType)
{
	return &_objectContainer[objType];
}

void ObjectManager::Zorder()
{
	//sort 정렬
	sort(_objectContainer[ObjectType::Object].begin(), 
		_objectContainer[ObjectType::Object].end(),
		[this](GameObject* object1, GameObject* object2) 
	{
		//오름차순 
		//만일 오브젝트(1) 충돌 렉트 바텀이 오브젝트(2) 충돌 렉트 바텀보다 더 위에 있으면
		if (object1->GetCollisonRect().bottom < object2->GetCollisonRect().bottom) 
		{
			//true 반환
			return true;
		}
		//flase 반환
		return false;
	});
}

void ObjectManager::ObjectRender()
{
	if (_isZorder == true) this->Zorder();

	ObjectContainerIter iter = _objectContainer.begin();
	for (; iter != _objectContainer.end(); ++iter) 
	{
		//만일 오브젝트 타입이 UI인 경우 continue하기
		if (iter->first == ObjectType::UI) continue;

		for (UINT i = 0; i < iter->second.size(); ++i) 
		{
			//만일 GetActive가 true이면
			if (iter->second[i]->GetActive()) 
			{
				//해당 인덱스 렌더
				iter->second[i]->Render();
			}
		}
	}
}

void ObjectManager::UIRender()
{
	//데미지 폰트 매니저 들어갈 예정
	
	//오브젝트 타입 UI의 size만큼 for문 돌기
	for (UINT i = 0; i < _objectContainer[ObjectType::UI].size(); ++i) 
	{
		//object는 _objectContainer[ObjectType::UI][i] == Vector의 i를 받는다.
		GameObject* object = _objectContainer[ObjectType::UI][i];
		
		//만일 GetActive가 true면
		if (object->GetActive()) 
		{
			//object 렌더
			object->Render();
		}
	}
}
