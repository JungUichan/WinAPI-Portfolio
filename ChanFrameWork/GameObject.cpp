#include "stdafx.h"
#include "GameObject.h"

/*************************************************************************
## GameObject ## ������
**************************************************************************/
GameObject::GameObject()
	:_name(""), _pivot(Pivot::Center), _position(0, 0), _size(0, 0), _isLive(true)
{
	this->UpdateMainRect();
}

/*************************************************************************
## GameObject ## ������
GameObject ���ڵ� �����ε�
**************************************************************************/
GameObject::GameObject(string name, Vector2 pos, Vector2 size, Pivot::Enum pivot)
	:_name(name), _position(pos), _size(size), _pivot(pivot), _isActive(true), _isLive(true)
{
	this->UpdateMainRect();
}

/*************************************************************************
## GameObject ## �Ҹ���
**************************************************************************/
GameObject::~GameObject()
{
	
}

/*************************************************************************
## GameObject ## Init
**************************************************************************/
void GameObject::Init()
{
}

/*************************************************************************
## GameObject ## Release
**************************************************************************/
void GameObject::Release()
{
}

/*************************************************************************
## GameObject ## Updates
**************************************************************************/
void GameObject::Update()
{
}

void GameObject::Render()
{
	if (_isDebug) 
	{
		_D2DRenderer->DrawRectangle(_mainRect, D2DRenderer::DefaultBrush::Red, true);
	}
}

void GameObject::SetPosition(const Vector2 position)
{
	this->_position = position;
	this->UpdateMainRect();
}

void GameObject::SetSize(const Vector2 size)
{
	this->_size = size;
	this->UpdateMainRect();
}

void GameObject::SetPivot(const Pivot::Enum pivot)
{
	this->_pivot = pivot;
	this->UpdateMainRect();
}

void GameObject::SetMainRect(const Figure::FloatRect rect)
{
	this->_mainRect = rect;
}

void GameObject::SetActive(const bool active)
{
	this->_isActive = active;
	if (_isActive == true)
		this->Enable();	//Ȱ��ȭ �Լ� ����
	else
		this->Disable();//��Ȱ��ȭ �Լ� ����
}

void GameObject::UpdateMainRect()
{
	switch (_pivot)
	{
	case Pivot::LeftTop:
		this->_mainRect = Figure::FloatRect(_position, _size, Pivot::LeftTop);
		//this->_mainRect.Update(_position, _size, Pivot::LeftTop);
		break;
	case Pivot::Center:
		this->_mainRect = Figure::FloatRect(_position, _size, Pivot::Center);
		break;
	case Pivot::Bottom:
		this->_mainRect = Figure::FloatRect(_position, _size, Pivot::Bottom);
		break;
	default:
		break;
	}
}
