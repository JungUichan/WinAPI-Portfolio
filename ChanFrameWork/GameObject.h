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

	//Ȱ��ȭ �� ����Ǵ� �Լ�
	virtual void Enable() {}

	//��Ȱ��ȭ �� ����Ǵ� �Լ�
	virtual void Disable() {}

	//�浹ó���� �Լ�
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

	//��� ������Ʈ ����
	void Destroy() { this->_isLive = false; }
	
protected:
	//��Ʈ ���� �� ��Ʈ ������Ʈ �ʿ� (Pivot�� ���� ��Ʈ ������Ʈ)
	void UpdateMainRect();
};

