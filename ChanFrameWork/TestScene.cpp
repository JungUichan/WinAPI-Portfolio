#include "stdafx.h"
#include "TestScene.h"
#include "GameObject.h"

void TestScene::Init()
{
	this->SceneInit();

	_ObjectManager->Init();

	_map = _ImageManager->FindImage("TestMap");

	_CameraManager->SetTarget(_ObjectManager->FindObject(ObjectType::Object, "Player"));
	_CameraManager->SetMapSize(Vector2((int)_map->GetWidth() * 2, (int)_map->GetHeight() * 2));
}

void TestScene::Release()
{
	_ObjectManager->Release();
}

void TestScene::Update()
{
	_ObjectManager->Update();
	_CameraManager->Update();
}

void TestScene::Render()
{

	_map->Render(0, 0, Pivot::LeftTop, false);
	_ObjectManager->Render();

	

	if (_isDebug) 
	{
		_D2DRenderer->DrawRectangle(Figure::FloatRect(Vector2(100, 100), Vector2(100, 100), Pivot::Center), D2DRenderer::DefaultBrush::Red, true);

	}

}

