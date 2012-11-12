#pragma once


template <class T>
class Vector3
{
public:
	typedef NumericTraits<T> Traits;

	Vector3()
		: _x(static_cast<T>(0)), _y(static_cast<T>(0)), _z(static_cast<T>(0)) {}

	Vector3(const Vector3& other)
		: _x(other._x), _y(other._y), _z(other._z) {}

	Vector3(T x, T y, T z)
		: _x(x), _y(y), _z(z) {}

	void set(T x, T y, T z)
	{
		_x = x;
		_y = y;
		_z = z;
	}
	
	T& x()
	{
		return _x;
	}

	T& y()
	{
		return _y;
	}

	T& z()
	{
		return _z;
	}

	T x() const
	{
		return _x;
	}

	T y() const
	{
		return _y;
	}

	T z() const
	{
		return _z;
	}

	Vector3& operator=(const Vector3& other)
	{
		_x = other._x;
		_y = other._y;
		_z = other._z;

		return *this;
	}

	Vector3 operator+(const Vector3& other) const
	{
		return Vector3(_x + other._x, _y + other._y, _z + other._z);
	}

	Vector3 operator-(const Vector3& other) const
	{
		return Vector3(_x - other._x, _y - other._y, _z - other._z);
	}

	Vector3 operator*(float scalar) const
	{
		return Vector3(_x * scalar, _y * scalar, _z * scalar);
	}

	T dot(const Vector3& other) const
	{
		return _x * other._x + _y * other._y + _z * other._z;
	}

	Vector3 cross(const Vector3& other) const
	{
		return Vector3(
			_y * other._z - _z * other._y,
			_z * other._x - _x * other._z,
			_x * other._y - _y * other._x
		);
	}

	Vector3& operator+=(const Vector3& other)
	{
		_x += other._x;
		_y += other._y;
		_z += other._z;

		return *this;
	}

	Vector3& operator-=(const Vector3& other)
	{
		_x -= other._x;
		_y -= other._y;
		_z -= other._z;

		return *this;
	}

	bool operator==(const Vector3& other) const
	{
		return _x == other._x && _y == other._y && _z == other._z;
	}

	bool operator!=(const Vector3& other) const
	{
		return _x != other._x || _y != other._y || _z != other._z;
	}

	void normalize()
	{
		T length = length();

		if (length > Traits::eps())
		{
			_x /= length;
			_y /= length;
			_z /= length;
		}
	}

	T length() const
	{
		return sqrt(_x * _x + _y * _y + _z * _z);
	}

private:
	T _x;
	T _y;
	T _z;
};
