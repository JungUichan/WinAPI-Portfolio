#pragma once
class GameObject
{
protected:
	string _name;
	Pivot::Enum _pivot;
	Vector2 _position;
	Vector2 _size;
	Figure::FloatRect _mainRect;
	bool _isActive;
private:
	bool _isLive;

public:
	GameObject();
	GameObject(string name, Vector2 pos, Vector2 size, Pivot::Enum pivot);
	virtual ~GameObject();

	virtual void Init() = 0;
	virtual void Release() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;

	//활성화 시 실행되는 함수
	virtual void Enable() {}

	//비활성화 시 실행되는 함수
	virtual void Disable() {}

	//충돌처리용 함수
	virtual Figure::FloatRect GetCollisonRect()const { return this->_mainRect; }

public:
	string GetName()const { return this->_name; }
	Vector2 GetPosition()const { return this->_position; }
	Vector2 GetSize()const { return this->_size; }
	Figure::FloatRect GetMainRect()const { return this->_mainRect; }
	bool GetActive()const { return this->_isActive; }
	bool GetLive()const { return this->_isLive; }

	void SetName(const string name) { this->_name = name; }
	void SetPosition(const Vector2 position);
	void SetSize(const Vector2 size);
	void SetPivot(const Pivot::Enum pivot);
	void SetMainRect(const Figure::FloatRect rect);
	void SetActive(const bool active);

	//대상 오브젝트 제거
	void Destroy() { this->_isLive = false; }
	
protected:
	//렉트 변동 시 렉트 업데이트 필요 (Pivot에 따라 렉트 업데이트)
	void UpdateMainRect();
};

