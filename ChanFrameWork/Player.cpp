#include "stdafx.h"
#include "Player.h"
#include "Animation.h"
#include <iostream>

Player::Player(Vector2 pos)
{
	//플레이어 동작 이미지
	this->_playerActionIMG = _ImageManager->FindImage("PlayerAction");


	this->_name = "Player";
	this->_position = pos;
	this->_isActive = true;
	this->_size = Vector2(90,75);	//원본 크기 30, 25
	this->_pivot = Pivot::Center;
	this->_mainRect = Figure::FloatRect(pos, _size,_pivot);
	this->_playerState = PlayerState::Down_Idle;
	
	this->_alpha = 1.0f;
	this->_stateCount = 0;

	this->UpdateMainRect();
	this->CreateAnimation();
	this->ChangeAnimation(PlayerState::Down_Idle);

	this->_moveSpeed = MoveSpeed;
	this->_dashSpeed = 1100.0f;
	
	this->_movePower = Vector2(0.f, 0.f);
}

Player::~Player()
{
}

void Player::Init()
{
}

void Player::Release()
{
	map<PlayerState, class Animation*>::iterator _aniIter = _aniList.begin();
	for (; _aniIter != _aniList.end(); ++_aniIter) 
	{
		SafeDelete(_aniIter->second);
	}
	_aniList.clear();
}

void Player::Update()
{
	//대각선 이동 시 속도 보정
	//if ((_Input->GetKey('A') && _Input->GetKey('W')) || (_Input->GetKey('A') && _Input->GetKey('S'))
	//	|| (_Input->GetKey('D') && _Input->GetKey('W')) || (_Input->GetKey('D') && _Input->GetKey('S')))
	//{
	//	cout << "revise" << endl;
	//	_moveSpeed = sqrtf((_moveSpeed * _moveSpeed) + (_moveSpeed * _moveSpeed));
	//}
	//else
	//{
	//	_moveSpeed = MoveSpeed;
	//}

	//초기화 해줘서 값 누적 방지
	_movePower = Vector2(0, 0);
	

	this->UpdateState();
	this->MoveRevise(_movePower);	//이동 보정 값 (대각선 보정 값 적용을 위해)
	this->UpdateMainRect();
	this->_playerAnimation->UpdateFrame();
}

void Player::Render()
{
	//플레이어 동작 이미지 사이즈
	this->_playerActionIMG->SetSize(_size);
	this->_playerActionIMG->SetAlpha(_alpha);

	//상태에 따른 이미지 렌더 처리
	switch (_playerState)
	{
	case Player::PlayerState::Down_Idle:
		this->_playerActionIMG->FrameRender(_position.x, _position.y, _playerAnimation->GetNowFrameX(),
			_playerAnimation->GetNowFrameY(), Pivot::Center, true);
		break;
	case Player::PlayerState::Left_Idle:
		this->_playerActionIMG->FrameRender(_position.x, _position.y, _playerAnimation->GetNowFrameX(),
			_playerAnimation->GetNowFrameY(), Pivot::Center, true);
		break;
	case Player::PlayerState::Right_Idle:
		this->_playerActionIMG->FrameRender(_position.x, _position.y, _playerAnimation->GetNowFrameX(),
			_playerAnimation->GetNowFrameY(), Pivot::Center, true);
		break;
	case Player::PlayerState::Up_Idle:
		this->_playerActionIMG->FrameRender(_position.x, _position.y, _playerAnimation->GetNowFrameX(),
			_playerAnimation->GetNowFrameY(), Pivot::Center, true);
		break;
	case Player::PlayerState::Left_Run:
		this->_playerActionIMG->FrameRender(_position.x, _position.y, _playerAnimation->GetNowFrameX(),
			_playerAnimation->GetNowFrameY(), Pivot::Center, true);
		break;
	case Player::PlayerState::Right_Run:
		this->_playerActionIMG->FrameRender(_position.x, _position.y, _playerAnimation->GetNowFrameX(),
			_playerAnimation->GetNowFrameY(), Pivot::Center, true);
		break;
	case Player::PlayerState::Up_Run:
		this->_playerActionIMG->FrameRender(_position.x, _position.y, _playerAnimation->GetNowFrameX(),
			_playerAnimation->GetNowFrameY(), Pivot::Center, true);
		break;
	case Player::PlayerState::Down_Run:
		this->_playerActionIMG->FrameRender(_position.x, _position.y, _playerAnimation->GetNowFrameX(),
			_playerAnimation->GetNowFrameY(), Pivot::Center, true);
		break;
	case Player::PlayerState::Left_Dash:
		this->_playerActionIMG->FrameRender(_position.x, _position.y, _playerAnimation->GetNowFrameX(),
			_playerAnimation->GetNowFrameY(), Pivot::Center, true);
		break;
	case Player::PlayerState::Right_Dash:
		this->_playerActionIMG->FrameRender(_position.x, _position.y, _playerAnimation->GetNowFrameX(),
			_playerAnimation->GetNowFrameY(), Pivot::Center, true);
		break;
	case Player::PlayerState::Up_Dash:
		this->_playerActionIMG->FrameRender(_position.x, _position.y, _playerAnimation->GetNowFrameX(),
			_playerAnimation->GetNowFrameY(), Pivot::Center, true);
		break;
	case Player::PlayerState::Down_Dash:
		this->_playerActionIMG->FrameRender(_position.x, _position.y, _playerAnimation->GetNowFrameX(),
			_playerAnimation->GetNowFrameY(), Pivot::Center, true);
		break;
	case Player::PlayerState::Left_Atk:
		this->_playerActionIMG->FrameRender(_position.x, _position.y, _playerAnimation->GetNowFrameX(),
			_playerAnimation->GetNowFrameY(), Pivot::Center, true);
		break;
	case Player::PlayerState::Right_Atk:
		this->_playerActionIMG->FrameRender(_position.x, _position.y, _playerAnimation->GetNowFrameX(),
			_playerAnimation->GetNowFrameY(), Pivot::Center, true);
		break;
	case Player::PlayerState::Up_Atk:
		this->_playerActionIMG->FrameRender(_position.x, _position.y, _playerAnimation->GetNowFrameX(), 
			_playerAnimation->GetNowFrameY(), Pivot::Center, true);
		break;
	case Player::PlayerState::Down_Atk:
		this->_playerActionIMG->FrameRender(_position.x, _position.y, _playerAnimation->GetNowFrameX(),
			_playerAnimation->GetNowFrameY(), Pivot::Center, true);
		break;
	case Player::PlayerState::End:
		break;
	default:
		break;
	}


	if (_isDebug)
	{
		_D2DRenderer->DrawRectangle(_mainRect, D2DRenderer::DefaultBrush::Green, true);
	}
}

/******************************************************
## ChangeState ##
@@ 상태를 찾아 해당하는 상태를 바꿈
*******************************************************/
void Player::ChangeState(PlayerState state)
{
	if (_playerState == state)	return;
	
	//입력한 state로 상태 변화
	_playerState = state;

	//상태가 바뀔 때 애니메이션도 같이 바뀌게 하기
	ChangeAnimation(state);

	switch (_playerState)
	{
	//기본 상태 - 
	case Player::PlayerState::Left_Idle:
		break;
	case Player::PlayerState::Right_Idle:
		break;
	case Player::PlayerState::Up_Idle:
		break;
	case Player::PlayerState::Down_Idle:
		break;
	
	//이동 상태 - 
	case Player::PlayerState::Left_Run:
		
		break;
	case Player::PlayerState::Right_Run:
		
		break;
	case Player::PlayerState::Up_Run:
	
		break;
	case Player::PlayerState::Down_Run:
		
		break;
	
	//대쉬 상태 - 
	case Player::PlayerState::Left_Dash:
		
		break;
	case Player::PlayerState::Right_Dash:
		
		break;
	case Player::PlayerState::Up_Dash:
		
		break;
	case Player::PlayerState::Down_Dash:
	
		break;
	//공격 상태 - 
	case Player::PlayerState::Left_Atk:
	
		break;
	case Player::PlayerState::Right_Atk:
	
		break;
	case Player::PlayerState::Up_Atk:
	
		break;
	case Player::PlayerState::Down_Atk:
	
		break;
	case Player::PlayerState::End:
		break;
	default:
		break;
	}
}
void Player::UpdateState()
{
	switch (_playerState)
	{
	case Player::PlayerState::Left_Idle:
		//아이들 상태에서 행동 처리 @@ 이동 조작 @@ 공격 @@
		_movePower += Vector2(0.f, 0.f);
		if (_Input->GetKey('A')) { ChangeState(PlayerState::Left_Run); }
		else if (_Input->GetKey('D')) { ChangeState(PlayerState::Right_Run); }
		else if (_Input->GetKey('W')) { ChangeState(PlayerState::Up_Run); }
		else if (_Input->GetKey('S')) { ChangeState(PlayerState::Down_Run); }
		else if (_Input->GetKeyDown('J')){ ChangeState(PlayerState::Left_Atk); }
		break;
	case Player::PlayerState::Right_Idle:
		_movePower += Vector2(0.f, 0.f);
		//아이들 상태에서 행동 처리 @@ 이동 조작 @@ 공격 @@
		if (_Input->GetKey('A')) { ChangeState(PlayerState::Left_Run); }
		else if (_Input->GetKey('D')) { ChangeState(PlayerState::Right_Run); }
		else if (_Input->GetKey('W')) { ChangeState(PlayerState::Up_Run); }
		else if (_Input->GetKey('S')) { ChangeState(PlayerState::Down_Run); }
		else if (_Input->GetKeyDown('J')) { ChangeState(PlayerState::Right_Atk); }
		break;
	case Player::PlayerState::Up_Idle:
		_movePower += Vector2(0.f, 0.f);
		//아이들 상태에서 행동 처리 @@ 이동 조작 @@ 공격 @@
		if (_Input->GetKey('A')) { ChangeState(PlayerState::Left_Run); }
		else if (_Input->GetKey('D')) { ChangeState(PlayerState::Right_Run); }
		else if (_Input->GetKey('W')) { ChangeState(PlayerState::Up_Run); }
		else if (_Input->GetKey('S')) { ChangeState(PlayerState::Down_Run); }
		else if (_Input->GetKeyDown('J')) { ChangeState(PlayerState::Up_Atk); }
		break;
	case Player::PlayerState::Down_Idle:
		_movePower += Vector2(0.f, 0.f);
		//아이들 상태에서 행동 처리 @@ 이동 조작 @@ 공격 @@
		if (_Input->GetKey('A')) { ChangeState(PlayerState::Left_Run); }
		else if (_Input->GetKey('D')) { ChangeState(PlayerState::Right_Run); }
		else if (_Input->GetKey('W')) { ChangeState(PlayerState::Up_Run); }
		else if (_Input->GetKey('S')) { ChangeState(PlayerState::Down_Run); }
		else if (_Input->GetKeyDown('J')) { ChangeState(PlayerState::Down_Atk); }
		break;
	//이동
	case Player::PlayerState::Left_Run:
		//이동 상태에서 행동 처리 @@ 공격 @@ 대쉬 @@
		_movePower += Vector2(-1.f, 0.f);
		if (_Input->GetKeyUp('A')) { ChangeState(PlayerState::Left_Idle); cout << "왼쪽 이동" << endl; }
		else if (_Input->GetKeyDown('W')) { ChangeAnimation(PlayerState::Up_Run); }
		else if (_Input->GetKey('W')) 
		{
			cout << "대각선 이동" << endl;
			_movePower += Vector2(0.f, -1.f);
			if (_Input->GetKeyDown(VK_SPACE)) { ChangeState(PlayerState::Up_Dash); }
			else if(_Input->GetKeyDown('J')) { ChangeState(PlayerState::Up_Atk); }
		}
		else if (_Input->GetKeyDown('S')) { ChangeAnimation(PlayerState::Down_Run); }
		else if (_Input->GetKey('S')) 
		{
			cout << "대각선 이동" << endl;
			_movePower += Vector2(0.f, 1.f);
			if (_Input->GetKeyDown(VK_SPACE)) { ChangeState(PlayerState::Down_Dash); }
			else if (_Input->GetKeyDown('J')) { ChangeState(PlayerState::Down_Atk); }
		}
		else if (_Input->GetKeyUp('S')) { ChangeAnimation(PlayerState::Left_Run); }
		else if (_Input->GetKeyUp('W')) { ChangeAnimation(PlayerState::Left_Run); }
		
		else if (_Input->GetKeyDown('J')) { ChangeState(PlayerState::Left_Atk); }
		else if (_Input->GetKeyDown(VK_SPACE)) { ChangeState(PlayerState::Left_Dash); }
		this->UpdateMainRect();
		break;
	case Player::PlayerState::Right_Run:
		//이동 상태에서 행동 처리 @@ 공격 @@ 대쉬 @@
		_movePower += Vector2(1.f, 0.f);
		if (_Input->GetKeyUp('D')){ ChangeState(PlayerState::Right_Idle); cout << "오른쪽 이동" << endl;	}
		else if (_Input->GetKeyDown('W')) { ChangeAnimation(PlayerState::Up_Run); }
		else if (_Input->GetKey('W'))
		{
			cout << "대각선 이동" << endl;
			_movePower += Vector2(0.f, -1.f);
			if (_Input->GetKeyDown(VK_SPACE)) { ChangeState(PlayerState::Up_Dash); }
			else if (_Input->GetKeyDown('J')) { ChangeState(PlayerState::Up_Atk); }
		}
		else if (_Input->GetKeyDown('S')) { ChangeAnimation(PlayerState::Down_Run); }
		else if (_Input->GetKey('S'))
		{
			cout << "대각선 이동" << endl;
			_movePower += Vector2(0.f, 1.f);
			if (_Input->GetKeyDown(VK_SPACE)) { ChangeState(PlayerState::Down_Dash); }
			else if (_Input->GetKeyDown('J')) { ChangeState(PlayerState::Down_Atk); }
		}
		else if (_Input->GetKeyUp('W')) { ChangeAnimation(PlayerState::Right_Run); }
		else if (_Input->GetKeyUp('S')) { ChangeAnimation(PlayerState::Right_Run); }

		else if (_Input->GetKeyDown('J')) { ChangeState(PlayerState::Right_Atk); }
		else if (_Input->GetKeyDown(VK_SPACE)) { ChangeState(PlayerState::Right_Dash); }
		this->UpdateMainRect();
		break;
	case Player::PlayerState::Up_Run:
		//이동 상태에서 행동 처리 @@ 공격 @@ 대쉬 @@
		_movePower += Vector2(0.f, -1.f);
		if (_Input->GetKeyUp('W')) { ChangeState(PlayerState::Up_Idle); cout << "위쪽 이동" << endl;	}
		else if (_Input->GetKeyDown('A')) { ChangeAnimation(PlayerState::Left_Run); }
		else if (_Input->GetKey('A'))
		{
			cout << "대각선 이동" << endl;
			_movePower += Vector2(-1.f, 0.f);
			if (_Input->GetKeyDown(VK_SPACE)) { ChangeState(PlayerState::Left_Dash); }
			else if (_Input->GetKeyDown('J')) { ChangeState(PlayerState::Up_Atk); }

		}
		else if (_Input->GetKeyDown('D')) { ChangeAnimation(PlayerState::Right_Run); }
		else if (_Input->GetKey('D'))
		{
			cout << "대각선 이동" << endl;
			_movePower += Vector2(1.f, 0.f);
			if (_Input->GetKeyDown(VK_SPACE)) { ChangeState(PlayerState::Right_Dash); }
			else if (_Input->GetKeyDown('J')) { ChangeState(PlayerState::Up_Atk); }
		}
		else if (_Input->GetKeyUp('A')) { ChangeAnimation(PlayerState::Up_Run); }
		else if (_Input->GetKeyUp('D')) { ChangeAnimation(PlayerState::Up_Run); }

		else if (_Input->GetKeyDown('J')) { ChangeState(PlayerState::Up_Atk); }
		else if (_Input->GetKeyDown(VK_SPACE)) { ChangeState(PlayerState::Up_Dash); }
		this->UpdateMainRect();
		break;
	case Player::PlayerState::Down_Run:
		//이동 상태에서 행동 처리 @@ 공격 @@ 대쉬 @@
		_movePower += Vector2(0.f, 1.f);
		if (_Input->GetKeyUp('S')) { ChangeState(PlayerState::Down_Idle); cout << "아래쪽 이동" << endl;	}
		else if(_Input->GetKeyDown('A')){ ChangeAnimation(PlayerState::Left_Run); }
		else if (_Input->GetKey('A'))
		{
			cout << "대각선 이동" << endl;
			_movePower += Vector2(-1.f, 0.f);
			if (_Input->GetKeyDown(VK_SPACE)) { ChangeState(PlayerState::Left_Dash); }
			else if (_Input->GetKeyDown('J')) { ChangeState(PlayerState::Down_Atk); }
			
		}
		else if(_Input->GetKeyDown('D')){ ChangeAnimation(PlayerState::Right_Run); }
		else if (_Input->GetKey('D'))
		{
			cout << "대각선 이동" << endl;
			_movePower += Vector2(1.f, 0.f);
			if (_Input->GetKeyDown(VK_SPACE)) { ChangeState(PlayerState::Right_Dash); }
			else if (_Input->GetKeyDown('J')) { ChangeState(PlayerState::Down_Atk); }
		}
		else if (_Input->GetKeyUp('A')) { ChangeAnimation(PlayerState::Down_Run); }
		else if (_Input->GetKeyUp('D')) { ChangeAnimation(PlayerState::Down_Run); }

		else if (_Input->GetKeyDown('J')) { ChangeState(PlayerState::Down_Atk); }
		else if (_Input->GetKeyDown(VK_SPACE)) { ChangeState(PlayerState::Down_Dash); }
		this->UpdateMainRect();
		break;

	//대쉬
	case Player::PlayerState::Left_Dash:
		_position.x -= _dashSpeed * _TimeManager->DeltaTime();
		_stateCount += _TimeManager->DeltaTime();
		
		//만일 상태 카운트가 0.1보다 커지면
		if (_stateCount >= 0.1f)
		{
			//속도를 점점 느리게 300씩 감소
			_dashSpeed -= 300.0f;
			//속도가 0보다 작거나 같아지면
			if (_dashSpeed <= 0)
			{
				//속도를 50으로 초기화
				_dashSpeed = 50.0f;
			}
		}
		//만일 상태 카운트가 StateCount보다 같거나 커지면
		if (_stateCount >= StateCount)
		{
			_dashSpeed = 1100.0f;
			_stateCount = 0;
			ChangeState(PlayerState::Left_Idle);
		}
		this->UpdateMainRect();
		break;
	case Player::PlayerState::Right_Dash:
		_position.x += _dashSpeed * _TimeManager->DeltaTime();
		_stateCount += _TimeManager->DeltaTime();
		//만일 상태 카운트가 0.1보다 커지면
		if (_stateCount >= 0.1f)
		{	
			//속도를 점점 느리게 300씩 감소
			_dashSpeed -= 300.0f;
			//속도가 0보다 작거나 같아지면
			if (_dashSpeed <= 0)
			{
				//속도를 50으로 초기화
				_dashSpeed = 50.0f;
			}
		}
		//만일 상태 카운트가 StateCount보다 같거나 커지면
		if (_stateCount >= StateCount)
		{
			_dashSpeed = 1100.0f;
			_stateCount = 0;
			ChangeState(PlayerState::Right_Idle);
		}
		this->UpdateMainRect();
		break;
	case Player::PlayerState::Up_Dash:
		_position.y -= _dashSpeed * _TimeManager->DeltaTime();
		_stateCount += _TimeManager->DeltaTime();
		//만일 상태 카운트가 0.1보다 커지면
		if (_stateCount >= 0.1f)
		{
			//속도를 점점 느리게 300씩 감소
			_dashSpeed -= 300.0f;
			//속도가 0보다 작거나 같아지면
			if (_dashSpeed <= 0)
			{
				//속도를 50으로 초기화
				_dashSpeed = 50.0f;
			}
		}
		//만일 상태 카운트가 StateCount보다 같거나 커지면
		if (_stateCount >= StateCount)
		{
			_dashSpeed = 1100.0f;
			_stateCount = 0;
			ChangeState(PlayerState::Up_Idle);
		}
		this->UpdateMainRect();
		break;
	case Player::PlayerState::Down_Dash:
		_position.y += _dashSpeed * _TimeManager->DeltaTime();
		_stateCount += _TimeManager->DeltaTime();
		//만일 상태 카운트가 0.1보다 커지면
		if (_stateCount >= 0.1f)
		{
			//속도를 점점 느리게 300씩 감소
			_dashSpeed -= 300.0f;
			//속도가 0보다 작거나 같아지면
			if (_dashSpeed <= 0)
			{
				//속도를 50으로 초기화
				_dashSpeed = 50.0f;
			}
		}
		//만일 상태 카운트가 StateCount보다 같거나 커지면
		if (_stateCount >= StateCount)
		{
			_dashSpeed = 1100.0f;
			_stateCount = 0;
			ChangeState(PlayerState::Down_Idle);
		}
		this->UpdateMainRect();
		break;

	//공격
	case Player::PlayerState::Left_Atk:
		break;
	case Player::PlayerState::Right_Atk:
		break;
	case Player::PlayerState::Up_Atk:
		break;
	case Player::PlayerState::Down_Atk:
		break;
	case Player::PlayerState::End:
		break;
	default:
		break;
	}
	
}
/******************************************************
## ChangeAnimation ##
@@ 애니메이션을 찾아 바꿈
*******************************************************/
void Player::ChangeAnimation(PlayerState state)
{
	map<PlayerState, class Animation*>::iterator aniIter;
	//애니메이션 리스트 중에 상태를 키 값으로 가진 애니메이션 찾기
	aniIter = _aniList.find(state);
	//만일 애니메이션 리스트를 찾으면
	if (aniIter != _aniList.end()) 
	{
		//찾은 애니메이션으로 바꾸기
		_playerAnimation = aniIter->second;
		_playerAnimation->Stop();
		_playerAnimation->Play();
	}
}
/******************************************************
## CreateAnimation ##
@@ 플레이어 상태에 따른 애니메이션 생성
*******************************************************/
void Player::CreateAnimation()
{
	//아래쪽 아이들
	Animation* downIdle = new Animation;
	downIdle->SetStartEndFrame(0, 0, 0, 0, false);
	downIdle->SetIsLoop(false);
	downIdle->SetFrameUpdateTime(IdleTime);
	_aniList.insert(make_pair(PlayerState::Down_Idle, downIdle));
	//오른쪽 아이들	
	Animation* rightIdle = new Animation;
	rightIdle->SetStartEndFrame(3, 0, 3, 0, false);
	rightIdle->SetIsLoop(false);
	rightIdle->SetFrameUpdateTime(IdleTime);
	_aniList.insert(make_pair(PlayerState::Right_Idle, rightIdle));
	//위쪽 아이들	
	Animation* upIdle = new Animation;
	upIdle->SetStartEndFrame(1, 0, 1, 0, false);
	upIdle->SetIsLoop(false);
	upIdle->SetFrameUpdateTime(IdleTime);
	_aniList.insert(make_pair(PlayerState::Up_Idle, upIdle));
	//왼쪽 아이들
	Animation* leftIdle = new Animation;
	leftIdle->SetStartEndFrame(2, 0, 2, 0, false);
	leftIdle->SetIsLoop(false);
	leftIdle->SetFrameUpdateTime(IdleTime);
	_aniList.insert(make_pair(PlayerState::Left_Idle, leftIdle));


	//왼쪽 이동
	Animation* leftRun = new Animation;
	leftRun->SetStartEndFrame(0, 3, 3, 3, false);
	leftRun->SetIsLoop(true);
	leftRun->SetFrameUpdateTime(RunTime);
	_aniList.insert(make_pair(PlayerState::Left_Run, leftRun));
	//오른쪽 이동
	Animation* rightRun = new Animation;
	rightRun->SetStartEndFrame(0, 4, 3, 4, false);
	rightRun->SetIsLoop(true);
	rightRun->SetFrameUpdateTime(RunTime);
	_aniList.insert(make_pair(PlayerState::Right_Run, rightRun));
	//위쪽 이동
	Animation* upRun = new Animation;
	upRun->SetStartEndFrame(0, 2, 3, 2, false);
	upRun->SetIsLoop(true);
	upRun->SetFrameUpdateTime(RunTime);
	_aniList.insert(make_pair(PlayerState::Up_Run, upRun));
	//아래쪽 이동
	Animation* downRun = new Animation;
	downRun->SetStartEndFrame(0, 1, 3, 1, false);
	downRun->SetIsLoop(true);
	downRun->SetFrameUpdateTime(RunTime);
	_aniList.insert(make_pair(PlayerState::Down_Run, downRun));


	//왼쪽 대쉬
	Animation* leftDash = new Animation;
	leftDash->SetStartEndFrame(0, 5, 3, 5, false);
	leftDash->SetIsLoop(true);
	leftDash->SetFrameUpdateTime(DashTime);
	_aniList.insert(make_pair(PlayerState::Left_Dash, leftDash));
	//오른쪽 대쉬
	Animation* rightDash = new Animation;
	rightDash->SetStartEndFrame(0, 6, 3, 6, false);
	rightDash->SetIsLoop(true);
	rightDash->SetFrameUpdateTime(DashTime);
	_aniList.insert(make_pair(PlayerState::Right_Dash, rightDash));
	//위쪽 대쉬
	Animation* upDash = new Animation;
	upDash->SetStartEndFrame(0, 7, 3, 7, false);
	upDash->SetIsLoop(true);
	upDash->SetFrameUpdateTime(DashTime);
	_aniList.insert(make_pair(PlayerState::Up_Dash, upDash));
	//아래쪽 대쉬
	Animation* downDash = new Animation;
	downDash->SetStartEndFrame(0, 8, 3, 8, false);
	downDash->SetIsLoop(true);
	downDash->SetFrameUpdateTime(DashTime);
	_aniList.insert(make_pair(PlayerState::Down_Dash, downDash));
	
	//왼쪽 공격
	Animation* leftAtk = new Animation;
	leftAtk->SetStartEndFrame(0, 9, 3, 9, false);
	leftAtk->SetIsLoop(false);
	leftAtk->SetFrameUpdateTime(AtkTime);
	leftAtk->SetCallbackFunc([this]() {this->ChangeState(PlayerState::Left_Idle); });
	_aniList.insert(make_pair(PlayerState::Left_Atk, leftAtk));
	//오른쪽 공격
	Animation* rightAtk = new Animation;
	rightAtk->SetStartEndFrame(0, 10, 3, 10, false);
	rightAtk->SetIsLoop(false);
	rightAtk->SetFrameUpdateTime(AtkTime);
	rightAtk->SetCallbackFunc([this]() {this->ChangeState(PlayerState::Right_Idle); });
	_aniList.insert(make_pair(PlayerState::Right_Atk, rightAtk));
	//위쪽 공격
	Animation* upAtk = new Animation;
	upAtk->SetStartEndFrame(0, 11, 3, 11, false);
	upAtk->SetIsLoop(false);
	upAtk->SetFrameUpdateTime(AtkTime);
	upAtk->SetCallbackFunc([this]() {this->ChangeState(PlayerState::Up_Idle); });
	_aniList.insert(make_pair(PlayerState::Up_Atk, upAtk));
	//아래쪽 공격
	Animation* downAtk = new Animation;
	downAtk->SetStartEndFrame(0, 12, 3, 12, false);
	downAtk->SetIsLoop(false);
	downAtk->SetFrameUpdateTime(AtkTime);
	downAtk->SetCallbackFunc([this]() {this->ChangeState(PlayerState::Down_Idle); });
	_aniList.insert(make_pair(PlayerState::Down_Atk, downAtk));

}

/******************************************************
## MoveControl ##
@@ 대각선 이동 보정 - 단위 벡터 활용
*******************************************************/
void Player::MoveRevise(Vector2 movePower)
{
	//이동하지 않았다면 Float_EQUAL에 소수점 오차값으로 확인하여 이동을 하지 않았다면 return으로 빠져나간다.
	if (FLOAT_EQUAL(movePower.x, 0.f) && FLOAT_EQUAL(movePower.y, 0.f)) return;

	//이동속도 보정 x, y
	this->_position += Vector2::Normalize(&movePower) * _moveSpeed * _TimeManager->DeltaTime();


	this->UpdateMainRect();
}
