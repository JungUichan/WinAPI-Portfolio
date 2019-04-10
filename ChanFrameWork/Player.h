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
	//�÷��̾� ���� �̳ѹ�
	enum class PlayerState :int 
	{
		Left_Idle = 0, Right_Idle, Up_Idle, Down_Idle,
		Left_Run, Right_Run, Up_Run, Down_Run,
		Left_Dash, Right_Dash, Up_Dash, Down_Dash,
		Left_Atk, Right_Atk, Up_Atk, Down_Atk,
		
		End

	};
private:
	PlayerState _playerState;	//�÷��̾� ����
	float _stateCount;			//���� ������ ���� ī��Ʈ

	map<PlayerState, class Animation*> _aniList;	//�÷��̾� ���¿� �ִϸ��̼� ������ ���� �� ����
	class Animation* _playerAnimation;				//�÷��̾� �ִϸ��̼�

	//�÷��̾� ���� �̹���
	class Image* _playerActionIMG;					//�÷��̾� ���� �̹���

	float _alpha;									//�÷��̾� �̹��� ���İ�

	float _dashSpeed;								//�÷��̾� �뽬 �ӵ�

	float _moveSpeed;								//�÷��̾� �̵� �ӵ�

	Vector2 _movePower;								//�����̴� �� x,y

public:
	Player(Vector2 pos);	//�÷��̾� ������ ��ǥ��
	~Player();

	void Init()override;
	void Release()override;
	void Update()override;
	void Render()override;


	//���� �ٲٱ�(�ٲ� �� �ѹ����� ����)
	void ChangeState(PlayerState state);
	//�ش�����ϋ� ��÷� ����.
	void UpdateState();
	//�ִϸ��̼� �ٲٱ�
	void ChangeAnimation(PlayerState state);
	//���¿� ���� �ִϸ��̼� ����
	void CreateAnimation();

	//�̵� ���� (�밢�� ���� ����)
	void MoveRevise(Vector2 movePower);
};

