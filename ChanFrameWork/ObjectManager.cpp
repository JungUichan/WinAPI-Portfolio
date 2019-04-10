#include "stdafx.h"
#include "ObjectManager.h"
#include "GameObject.h"
#include <algorithm>
#include <unordered_map>

/*********************************************************************
## ObjectManager ##
-> ������ 
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
-> �Ҹ���, ��� ������Ʈ �޸� ���� �� �����̳� Ŭ����
**********************************************************************/
ObjectManager::~ObjectManager()
{
	//�������� ���� �Ҹ��ڿ��� �ѹ� �� Release
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
			//���� GetLive�� false���
			if (iter->second[i]->GetLive() == false) 
			{
				//�ش� �ε����� Release��Ű��
				iter->second[i]->Release();
				//�ش� �ε����� SafeDelete��Ű��
				SafeDelete(iter->second[i]);
				//�ش� �ε����� erase�� �����
				//�޸� ���� �� ���� �� null ������Ʈ ��ü ����
				//������ �� �����ϱ� ���� ���� i-- 
				iter->second.erase(iter->second.begin() + i--);
				continue;
			}
			//���� GetActive�� true���
			if (iter->second[i]->GetActive()) 
			{
				//�ش� �ε��� Update ����
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
	//������Ʈ ����
	this->ObjectRender();
	//UI ����
	this->UIRender();
}

/************************************************************************************
## AddObject ##
-> ������Ʈ ��� �Լ�
: ������Ʈ Ÿ�԰� ������Ʈ 2���� ���ڸ� �޾Ƽ� ���
*************************************************************************************/
void ObjectManager::AddObject(const ObjectType::Enum objType, GameObject* pGameObject)
{
	this->_objectContainer[objType].push_back(pGameObject);
}

/************************************************************************************
## FindObject ##
-> ����� ������Ʈ ã�Ƽ� �������� �Լ�
: ������Ʈ Ÿ�԰� �ش� ������Ʈ Ű �� 2���� ���ڸ� �޾Ƽ� ���
*************************************************************************************/
GameObject* ObjectManager::FindObject(const ObjectType::Enum objType, const string keyName)
{
	//������Ʈ Ÿ���� ã�Ƽ� ��������
	ObjectContainerIter iter = _objectContainer.find(objType);
	for (UINT i = 0; i < iter->second.size(); ++i) 
	{
		//## ������ Ÿ���� �ε��� Ž�� ##
		//���� �ش� �ε����� �Է��� Ű ���� ���ٸ�
		if (iter->second[i]->GetName() == keyName) 
		{
			//�ش� �ε��� ��ȯ
			return iter->second[i];
		}
	}
	return nullptr;
}
/************************************************************************************
## FindObjects ##
-> ����� ������Ʈ���� ã�Ƽ� �������� �Լ�
: ������Ʈ Ÿ�԰� �ش� ������Ʈ Ű �� 2���� ���ڸ� �޾Ƽ� ���
*************************************************************************************/
vector<class GameObject*> ObjectManager::FindObjects(const ObjectType::Enum objType, const string keyName)
{
	//������Ʈ Ÿ���� ã�Ƽ� ��������
	vector<GameObject*> findList;
	ObjectContainerIter iter = _objectContainer.find(objType);
	for (UINT i = 0; i < iter->second.size(); ++i) 
	{
		//## ������ Ÿ���� �ε��� Ž�� ##
		//���� �ش� �ε����� �Է��� Ű ���� ���ٸ�
		if (iter->second[i]->GetName() == keyName) 
		{
			//findList���Ϳ� Ž���ؼ� ã�� �ε������� push_back���� ���
			findList.push_back(iter->second[i]);
		}
	}
	//ã�Ƽ� ����� findList�� ��ȯ
	return findList;
}
/************************************************************************************
## GetObjectList ##
-> ����� ������Ʈ Ÿ���� ���� ã�Ƽ� �������� �Լ�
: ������Ʈ Ÿ������ ���ڸ� �޾Ƽ� ���
*************************************************************************************/
vector<class GameObject*> ObjectManager::GetObjectList(const ObjectType::Enum objType)
{
	return _objectContainer[objType];
}

/************************************************************************************
## GetObjectListPointer ##
-> ����� ������Ʈ Ÿ���� ���� ã�Ƽ� ���� �����ͷ� �������� �Լ�
: ������Ʈ Ÿ������ ���ڸ� �޾Ƽ� ���
: ���� �������� ���� ����� ���귮�� �����ͷ� ������ �ּҸ� ������ �������� �������� ���� ����
: const�� ���� ������ �ܺο��� �����Ͽ� �ٲ� ����� �ֱ⿡ �������� ���� ��� 
*************************************************************************************/
const vector<class GameObject*>* ObjectManager::GetObjectListPointer(const ObjectType::Enum objType)
{
	return &_objectContainer[objType];
}

void ObjectManager::Zorder()
{
	//sort ����
	sort(_objectContainer[ObjectType::Object].begin(), 
		_objectContainer[ObjectType::Object].end(),
		[this](GameObject* object1, GameObject* object2) 
	{
		//�������� 
		//���� ������Ʈ(1) �浹 ��Ʈ ������ ������Ʈ(2) �浹 ��Ʈ ���Һ��� �� ���� ������
		if (object1->GetCollisonRect().bottom < object2->GetCollisonRect().bottom) 
		{
			//true ��ȯ
			return true;
		}
		//flase ��ȯ
		return false;
	});
}

void ObjectManager::ObjectRender()
{
	if (_isZorder == true) this->Zorder();

	ObjectContainerIter iter = _objectContainer.begin();
	for (; iter != _objectContainer.end(); ++iter) 
	{
		//���� ������Ʈ Ÿ���� UI�� ��� continue�ϱ�
		if (iter->first == ObjectType::UI) continue;

		for (UINT i = 0; i < iter->second.size(); ++i) 
		{
			//���� GetActive�� true�̸�
			if (iter->second[i]->GetActive()) 
			{
				//�ش� �ε��� ����
				iter->second[i]->Render();
			}
		}
	}
}

void ObjectManager::UIRender()
{
	//������ ��Ʈ �Ŵ��� �� ����
	
	//������Ʈ Ÿ�� UI�� size��ŭ for�� ����
	for (UINT i = 0; i < _objectContainer[ObjectType::UI].size(); ++i) 
	{
		//object�� _objectContainer[ObjectType::UI][i] == Vector�� i�� �޴´�.
		GameObject* object = _objectContainer[ObjectType::UI][i];
		
		//���� GetActive�� true��
		if (object->GetActive()) 
		{
			//object ����
			object->Render();
		}
	}
}
