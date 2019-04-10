#pragma once
#include "GameObject.h"

#define MoveSpeed 250.0f

#define IdleTime 0.05f
#define RunTime 0.1f
#define DashTime 0.1f
#define AtkTime 0.05f

#define StateCount 0.5f

class Player : public GameObject
{

private:
	//플레이어 상태 이넘문
	enum class PlayerState :int 
	{
		Left_Idle = 0, Right_Idle, Up_Idle, Down_Idle,
		Left_Run, Right_Run, Up_Run, Down_Run,
		Left_Dash, Right_Dash, Up_Dash, Down_Dash,
		Left_Atk, Right_Atk, Up_Atk, Down_Atk,
		
		End

	};
private:
	PlayerState _playerState;	//플레이어 상태
	float _stateCount;			//상태 변경을 위한 카운트

	map<PlayerState, class Animation*> _aniList;	//플레이어 상태와 애니메이션 연동을 위한 맵 선언
	class Animation* _playerAnimation;				//플레이어 애니메이션

	//플레이어 동작 이미지
	class Image* _playerActionIMG;					//플레이어 동작 이미지

	float _alpha;									//플레이어 이미지 알파값

	float _dashSpeed;								//플레이어 대쉬 속도

	float _moveSpeed;								//플레이어 이동 속도

	Vector2 _movePower;								//움직이는 힘 x,y

public:
	Player(Vector2 pos);	//플레이어 생성할 좌표용
	~Player();

	void Init()override;
	void Release()override;
	void Update()override;
	void Render()override;


	//상태 바꾸기(바뀔때 단 한번씩만 실행)
	void ChangeState(PlayerState state);
	//해당상태일떄 상시로 실행.
	void UpdateState();
	//애니메이션 바꾸기
	void ChangeAnimation(PlayerState state);
	//상태에 따른 애니메이션 생성
	void CreateAnimation();

	//이동 보정 (대각선 단위 벡터)
	void MoveRevise(Vector2 movePower);
};

