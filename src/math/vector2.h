#pragma once


#include "numeric_traits.h"


template <class T>
class Vector2
{
public:
	typedef NumericTraits<T> Traits;

	Vector2() : _x(static_cast<T>(0)), _y(static_cast<T>(0)) {}
	
	Vector2(const Vector2& v)
	{
		_x = v._x;
		_y = v._y;
	}

	Vector2(float x, float y) : _x(static_cast<T>(x)), _y(static_cast<T>(y)) {}
	Vector2(int x, int y) : _x(static_cast<T>(x)), _y(static_cast<T>(y)) {}
	Vector2(unsigned x, unsigned y) : _x(static_cast<T>(x)), _y(static_cast<T>(y)) {}
	
	Vector2& operator=(const Vector2& v)
	{
		_x = v._x;
		_y = v._y;

		return *this;
	}

	void set(T x, T y)
	{
		_x = x;
		_y = y;
	}
	
	void set(int x, int y)
	{
		_x = static_cast<T>(x);
		_y = static_cast<T>(y);
	}

	bool operator==(const Vector2& v) const
	{
		return (_x == v._x) && (_y == v._y);
	}

	bool operator!=(const Vector2& v) const
	{
		return (_x != v._x) || (_y != v._y);
	}

	Vector2 operator+(const Vector2& v) const
	{
		return Vector2(_x + v._x, _y + v._y);
	}

	Vector2 operator-(const Vector2& v) const
	{
		return Vector2(_x - v._x, _y - v._y);
	}

	Vector2& operator+=(const Vector2& v)
	{
		_x += v._x;
		_y += v._y;

		return *this;
	}

	Vector2& operator-=(const Vector2& v)
	{
		_x -= v._x;
		_y -= v._y;

		return *this;
	}

	Vector2 operator*(T s) const
	{
		return Vector2(_x * s, _y * s);
	}

	Vector2& operator*=(T s)
	{
		_x *= s;
		_y *= s;

		return *this;
	}

	T dot(const Vector2& other) const
	{
		return _x * other._x + _y * other._y;
	}

	T x() const { return _x; }

	T y() const { return _y; }

	T& x() { return _x; }

	T& y() { return _y; }
	
	T getX() const { return _x; }
	void setX(T value) { _x = value; }

	T getY() const { return _x; }
	void setY(T value) { _x = value; }

	// return value is in [0, 2*PI]
	float direction() const
	{
		float angle = atan2(_y, _x);
		if (angle < 0)
		{
			angle += 2.0f * static_cast<float>(M_PI);
		}

		return angle;
	}

	float angle(const Vector2& v)
	{
		float theta1 = atan2(_y, _x);
		float theta2 = atan2(v._y, v._x);
		float d = theta2 - theta1;

		while (d > static_cast<float>(M_PI))
		{
			d -= 2.0f * static_cast<float>(M_PI);
		}

		while (d < -M_PI)
		{
			d += 2.0f * static_cast<float>(M_PI);
		}

		return d;
	}

	void rotate(float angleRad)
	{
		float sa = sin(angleRad);
		float ca = cos(angleRad);
		float x = _x * ca - _y * sa;
		float y = _y * ca + _x * sa;

		_x = x;
		_y = y;
	}

	T length() const
	{
		return sqrt(_x * _x + _y * _y);
	}

	T squareLength() const
	{
		return _x * _x + _y * _y;
	}

	void normalize()
	{
		T l = length();
		
		if (l > Traits::eps())
		{
			_x /= l;
			_y /= l;
		}
	}

private:
	T _x;
	T _y;
};
