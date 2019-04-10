#pragma once
class PlayerState
{
protected:
	class Player* _pPlayer;
	
public:
	PlayerState();
	~PlayerState();

	virtual void Enter() = 0;
	virtual void Update() = 0;
};

