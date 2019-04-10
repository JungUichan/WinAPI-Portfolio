#pragma once
#include "PlayerState.h"
class PlayerState_Idle : public PlayerState
{
private:
	class Player* _pPlayer;

public:
	PlayerState_Idle();
	~PlayerState_Idle();

	void Enter()override;
	void Update()override;

	void Render();
};

