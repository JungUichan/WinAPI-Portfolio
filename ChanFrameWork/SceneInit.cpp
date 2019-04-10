#include "stdafx.h"
#include "TestScene.h"
#include "LoadingScene.h"
#include "Player.h"

//�̰����� �ҷ��� ������ & ���ҽ� �̹��� �߰��ؼ� ����
void LoadingScene::LoadResource()
{
	//�÷��̾� ĳ���� ����
	_ImageManager->AddFrameImage("PlayerAction", L"../Resource/Character/Player_Action.png", 4, 14);

	//�÷��̾� ĳ���� �̵� ����
	_ImageManager->AddFrameImage("PlayerLeftRun", L"../Resource/Character/�ٱ�_����.png", 4, 1);
	_ImageManager->AddFrameImage("PlayerRightRun", L"../Resource/Character/�ٱ�_������.png", 4, 1);
	_ImageManager->AddFrameImage("PlayerUpRun", L"../Resource/Character/�ٱ�_��.png", 4, 1);
	_ImageManager->AddFrameImage("PlayerDownRun", L"../Resource/Character/�ٱ�_�Ʒ�.png", 4, 1);
	//�÷��̾� ĳ���� �뽬 ����
	_ImageManager->AddFrameImage("PlayerLeftDash", L"../Resource/Character/�뽬_����.png", 4, 1);
	_ImageManager->AddFrameImage("PlayerRightDash", L"../Resource/Character/�뽬_������.png", 4, 1);
	_ImageManager->AddFrameImage("PlayerUpDash", L"../Resource/Character/�뽬_��.png", 4, 1);
	_ImageManager->AddFrameImage("PlayerDownDash", L"../Resource/Character/�뽬_�Ʒ�.png", 4, 1);
	//�÷��̾� ĳ���� ���̵� ����
	_ImageManager->AddFrameImage("PlayerIdle", L"../Resource/Character/���̵�_�Ͽ����.png", 1, 4);
	//�÷��̾� ĳ���� ���� ����
	_ImageManager->AddImage("PlayerDie", L"../Resource/Character/����_����Ʈ.png");
	//�÷��̾� ĳ���� ���� ����
	_ImageManager->AddFrameImage("PlayerDownAttack", L"../Resource/Character/����_�Ʒ�.png", 4, 1);
	_ImageManager->AddFrameImage("PlayerUpAttack", L"../Resource/Character/����_��.png", 4, 1);
	_ImageManager->AddFrameImage("PlayerLeftAttack", L"../Resource/Character/����_����.png", 4, 1);
	_ImageManager->AddFrameImage("PlayerRightAttack", L"../Resource/Character/����_������.png", 4, 1);

	//�׽�Ʈ �� ���
	_ImageManager->AddImage("TestMap", L"../Resource/Map/TestMap.png");

}

void TestScene::SceneInit()
{
	/********************************************
	 ## ������Ʈ �Ŵ��� ��� ##
	*********************************************/
	_ObjectManager->AddObject(ObjectType::Object, new Player(Vector2(_WinSizeX / 2, _WinSizeY / 2)));
}



