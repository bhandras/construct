#pragma once


template <class T>
class Vector4
{
public:
	typedef NumericTraits<T> Traits;

	Vector4()
		: _x(static_cast<T>(0)), _y(static_cast<T>(0)), _z(static_cast<T>(0)), _w(static_cast<T>(0)) {}

	Vector4(const Vector4& other)
		: _x(other._x), _y(other._y), _z(other._z), _w(other._w) {}

	Vector4(T x, T y, T z, T w)
		: _x(x), _y(y), _z(z), _w(w) {}

	void set(T x, T y, T z, T w)
	{
		_x = x;
		_y = y;
		_z = z;
		_w = w;
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

	T& w()
	{
		return _w;
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

	T w() const
	{
		return _w;
	}

	Vector4& operator=(const Vector4& other)
	{
		_x = other._x;
		_y = other._y;
		_z = other._z;
		_w = other._w;

		return *this;
	}

	Vector4 operator+(const Vector4& other) const
	{
		return Vector4(_x + other._x, _y + other._y, _z + other._z, _w + other._w);
	}

	Vector4 operator-(const Vector4& other) const
	{
		return Vector4(_x - other._x, _y - other._y, _z - other._z, _w - other._w);
	}

	Vector4 operator*(float scalar) const
	{
		return Vector4(_x * scalar, _y * scalar, _z * scalar, _w * scalar);
	}

	T dot(const Vector4& other) const
	{
		return _x * other._x + _y * other._y + _z * other._z + _w * other._w;
	}

	Vector4& operator+=(const Vector4& other)
	{
		_x += other._x;
		_y += other._y;
		_z += other._z;
		_w += other._w;

		return *this;
	}

	Vector4& operator-=(const Vector4& other)
	{
		_x -= other._x;
		_y -= other._y;
		_z -= other._z;
		_w -= other._w;

		return *this;
	}

	bool operator==(const Vector4& other) const
	{
		return _x == other._x && _y == other._y && _z == other._z && _w == other._w;
	}

	bool operator!=(const Vector4& other) const
	{
		return _x != other._x || _y != other._y || _z != other._z || _w != other._w;
	}

	void normalize()
	{
		T l = length();

		if (length > Traits::eps())
		{
			_x /= l;
			_y /= l;
			_z /= l;
			_w /= l;
		}
	}

	T length() const
	{
		return sqrt(_x * _x + _y * _y + _z * _z + _w * _w);
	}

private:
	T _x;
	T _y;
	T _z;
	T _w;
};
