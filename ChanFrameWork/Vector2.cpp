#include "stdafx.h"
#include "Vector2.h"

/****************************************************************************************************
## Vector2 ##
*****************************************************************************************************/
Vector2::Vector2()
	:x(0.f), y(0.f) {}

Vector2::Vector2(const float& x, const float& y)
	: x(x), y(y) {}

Vector2::Vector2(const int& x, const int& y)
	: x((float)x), y((float)y) {}

Vector2::Vector2(const POINT& pt)
	: x((float)pt.x), y((float)pt.y) {}

/****************************************************************************************************
## GetIntX ##
@@ return int : int 형 x
*****************************************************************************************************/
int Vector2::GetIntX()
{
	return static_cast<int>(x);
}
/****************************************************************************************************
## GetIntY ##
@@ return int : int형 y 
*****************************************************************************************************/
int Vector2::GetIntY()
{
	return static_cast<int>(y);
}
/****************************************************************************************************
## GetPoint ##
@@ return POINT : POINT 형 Vector2 
*****************************************************************************************************/
POINT Vector2::GetPoint()
{
	return { (LONG)x,(LONG)y };
}
/****************************************************************************************************
## Operator oveloading ##
*****************************************************************************************************/
Vector2 Vector2::operator+(const Vector2 & v)
{
	Vector2 result;
	result.x = this->x + v.x;
	result.y = this->y + v.y;
	return result;
}

Vector2 Vector2::operator-(const Vector2 & v)
{
	Vector2 result;
	result.x = this->x - v.x;
	result.y = this->y - v.y;
	return result;
}

void Vector2::operator+=(const Vector2 & v)
{
	this->x += v.x;
	this->y += v.y;
}

void Vector2::operator-=(const Vector2 & v)
{
	this->x -= v.x;
	this->y -= v.y;
}

Vector2 Vector2::operator*(const float & scalar)
{
	Vector2 result;
	result.x = this->x * scalar;
	result.y = this->y * scalar;
	return result;
}

Vector2 Vector2::operator/(const float & scalar)
{
	Vector2 result;
	result.x = this->x / scalar;
	result.y = this->y / scalar;
	return result;
}

const bool Vector2::operator==(const Vector2 & v)
{
	if (this->x == v.x)
	{
		if (this->y == v.y)
			return true;
	}
	return false;
}

const bool Vector2::operator!=(const Vector2 & v)
{
	return !((*this) == v);
}

/****************************************************************************************************
## Static Normalize ##
@@ Vector2* pVector2 : 정규화 할 벡터 

@@ return Vector2 : 정규화된 벡터 
*****************************************************************************************************/
Vector2 Vector2::Normalize(const Vector2 * const pVector2)
{
	Vector2 result = *pVector2;

	float length = sqrt(result.x * result.x + result.y * result.y);
	result.x /= length;
	result.y /= length;

	return result;
}
float Vector2::NormalizeX(const float * const f1, const float * const f2)
{
	float resultX = *f1;
	float resultY = *f2;

	float length = sqrt(resultX * resultX + resultY * resultY);
	
	resultX /= length;

	return resultX;

}
float Vector2::NormalizeY(const float * const f1, const float * const f2)
{
	float resultX = *f1;
	float resultY = *f2;

	float length = sqrt(resultX * resultX + resultY * resultY);

	resultY /= length;

	return resultY;
}
/****************************************************************************************************
## static ToRadian ##
@@ Vector2* pVector2 : 라디안으로 변환할 벡터

@@ return float : 해당 벡터 라디안으로 변환한 값 
*****************************************************************************************************/
float Vector2::ToRadian(const Vector2 * const pVector2)
{
	int sign = 1;
	Vector2 dir = *pVector2;
	dir = Vector2::Normalize(&dir);
	(dir.y > 0.f) ? sign = -1 : sign = 1;
	return ((float)sign) * (acos(dir.x));
}
/****************************************************************************************************
## static Length ##
@@ Vector2* pVector2 : 크기 구할 벡터

@@ return float : 벡터의 크기 
*****************************************************************************************************/
float Vector2::Length(const Vector2* const pVector2)
{
	return sqrt(pVector2->x * pVector2->x + pVector2->y * pVector2->y);
}
/****************************************************************************************************
## static Dot ##
@@ Vector2* pVector2A : 벡터 A
@@ Vector2* pVector2B : 벡터 B

@@ return float : 두 벡터의 내적 값
*****************************************************************************************************/
float Vector2::Dot(const Vector2 * const pVector2A, const Vector2 * const pVector2B)
{
	return pVector2A->x * pVector2B->x + pVector2A->y * pVector2B->y;
}

