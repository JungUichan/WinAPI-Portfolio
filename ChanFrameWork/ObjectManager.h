#pragma once
class ObjectManager final : public Singletonbase<ObjectManager>
{
private:
	typedef unordered_map<ObjectType::Enum, vector<class GameObject*>> ObjectContainer;
	typedef unordered_map<ObjectType::Enum, vector<class GameObject*>>::iterator ObjectContainerIter;

private:
	ObjectContainer _objectContainer;
	
	bool _isZorder;

public:
	ObjectManager();
	~ObjectManager();

	void Init();
	void Release();
	void Update();
	void Render();

	void AddObject(const ObjectType::Enum objType, class GameObject* pGameObject);
	class GameObject* FindObject(const ObjectType::Enum objType, const string keyName);
	vector<class GameObject*> FindObjects(const ObjectType::Enum objType, const string keyName);
	vector<class GameObject*> GetObjectList(const ObjectType::Enum objType);
	const vector<class GameObject*>* GetObjectListPointer(const ObjectType::Enum objType);

	void ChangeZordering(bool zorder) { _isZorder = zorder; }

private:
	void Zorder();

	void ObjectRender();
	void UIRender();

};

#define _ObjectManager ObjectManager::Get()
