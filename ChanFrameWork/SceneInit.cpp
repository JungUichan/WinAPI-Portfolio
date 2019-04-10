#include "stdafx.h"
#include "TestScene.h"
#include "LoadingScene.h"
#include "Player.h"

//이곳에서 불러올 데이터 & 리소스 이미지 추가해서 관리
void LoadingScene::LoadResource()
{
	//플레이어 캐릭터 동작
	_ImageManager->AddFrameImage("PlayerAction", L"../Resource/Character/Player_Action.png", 4, 14);

	//플레이어 캐릭터 이동 조작
	_ImageManager->AddFrameImage("PlayerLeftRun", L"../Resource/Character/뛰기_왼쪽.png", 4, 1);
	_ImageManager->AddFrameImage("PlayerRightRun", L"../Resource/Character/뛰기_오른쪽.png", 4, 1);
	_ImageManager->AddFrameImage("PlayerUpRun", L"../Resource/Character/뛰기_위.png", 4, 1);
	_ImageManager->AddFrameImage("PlayerDownRun", L"../Resource/Character/뛰기_아래.png", 4, 1);
	//플레이어 캐릭터 대쉬 조작
	_ImageManager->AddFrameImage("PlayerLeftDash", L"../Resource/Character/대쉬_왼쪽.png", 4, 1);
	_ImageManager->AddFrameImage("PlayerRightDash", L"../Resource/Character/대쉬_오른쪽.png", 4, 1);
	_ImageManager->AddFrameImage("PlayerUpDash", L"../Resource/Character/대쉬_위.png", 4, 1);
	_ImageManager->AddFrameImage("PlayerDownDash", L"../Resource/Character/대쉬_아래.png", 4, 1);
	//플레이어 캐릭터 아이들 상태
	_ImageManager->AddFrameImage("PlayerIdle", L"../Resource/Character/아이들_하우상좌.png", 1, 4);
	//플레이어 캐릭터 죽음 상태
	_ImageManager->AddImage("PlayerDie", L"../Resource/Character/죽음_디폴트.png");
	//플레이어 캐릭터 공격 조작
	_ImageManager->AddFrameImage("PlayerDownAttack", L"../Resource/Character/공격_아래.png", 4, 1);
	_ImageManager->AddFrameImage("PlayerUpAttack", L"../Resource/Character/공격_위.png", 4, 1);
	_ImageManager->AddFrameImage("PlayerLeftAttack", L"../Resource/Character/공격_왼쪽.png", 4, 1);
	_ImageManager->AddFrameImage("PlayerRightAttack", L"../Resource/Character/공격_오른쪽.png", 4, 1);

	//테스트 맵 등록
	_ImageManager->AddImage("TestMap", L"../Resource/Map/TestMap.png");

}

void TestScene::SceneInit()
{
	/********************************************
	 ## 오브젝트 매니저 등록 ##
	*********************************************/
	_ObjectManager->AddObject(ObjectType::Object, new Player(Vector2(_WinSizeX / 2, _WinSizeY / 2)));
}



