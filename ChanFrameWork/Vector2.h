#pragma once
/****************************************************************************
## Vector2 ##
@@ Author : �Ǽ��� , Date : 2019.03.19
*****************************************************************************/
class Vector2 final
{
public:
	float x;
	float y; 
public:
	explicit Vector2();
	explicit Vector2(const float& x, const float& y);
	explicit Vector2(const int& x, const int& y);
	explicit Vector2(const POINT& pt);

	int GetIntX();
	int GetIntY();
	POINT GetPoint();

	Vector2 operator+ (const Vector2& v);
	Vector2 operator- (const Vector2& v);
	void operator+= (const Vector2& v);
	void operator-= (const Vector2& v);
	Vector2 operator* (const float& scalar);
	Vector2 operator/ (const float& scalar);
	const bool operator==(const Vector2& v);
	const bool operator!=(const Vector2& v);
public:
	static Vector2 Normalize(const Vector2* const  pVector2);
	static float NormalizeX(const float* const f1, const float* const f2);
	static float NormalizeY(const float* const f1, const float* const f2);
	static float ToRadian(const Vector2* const pVector2);
	static float Length(const Vector2* const pVector2);
	static float Dot(const Vector2* const pVector2A, const Vector2* const pVector2B);
};

