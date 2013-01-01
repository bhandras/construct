#pragma once


namespace Construct
{
	template <class T>
	class Vector2
	{
	public:
		typedef NumericTraits<T> Traits;
		T x;
		T y;

		Vector2()
			: x(static_cast<T>(0)), y(static_cast<T>(0)) {}

		Vector2(const Vector2& v)
		{
			x = v.x;
			y = v.y;
		}

		Vector2(float _x, float _y)
			: x(static_cast<T>(_x)), y(static_cast<T>(_y)) {}

		void set(T _x, T _y)
		{
			x = _x;
			y = _y;
		}

		Vector2& operator=(const Vector2& v)
		{
			x = v.x;
			y = v.y;

			return *this;
		}

		bool operator==(const Vector2& v) const
		{
			return (x == v.x) && (y == v.y);
		}

		bool operator!=(const Vector2& v) const
		{
			return (x != v.x) || (y != v.y);
		}

		Vector2 operator+(const Vector2& v) const
		{
			return Vector2(x + v.x, y + v.y);
		}

		Vector2 operator-(const Vector2& v) const
		{
			return Vector2(x - v.x, y - v.y);
		}

		Vector2& operator+=(const Vector2& v)
		{
			x += v.x;
			y += v.y;

			return *this;
		}

		Vector2& operator-=(const Vector2& v)
		{
			x -= v.x;
			y -= v.y;

			return *this;
		}

		Vector2 operator*(T s) const
		{
			return Vector2(x * s, y * s);
		}

		Vector2& operator*=(T s)
		{
			x *= s;
			y *= s;

			return *this;
		}

		Vector2 operator/(T s) const
		{
			return Vector2(x / s, y / s);
		}

		Vector2& operator/=(T s)
		{
			x /= s;
			y /= s;

			return *this;
		}

		T cross(const Vector2& other) const
		{
			return x * other.y - y * other.x;
		}

		T dot(const Vector2& other) const
		{
			return x * other.x + y * other.y;
		}

		// return value is in [0, 2*PI]
		float direction() const
		{
			float angle = atan2(y, x);
			if (angle < 0)
			{
				angle += 2.0f * static_cast<float>(M_PI);
			}

			return angle;
		}

		float angle(const Vector2& v)
		{
			float theta1 = atan2(y, x);
			float theta2 = atan2(v.y, v.x);
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
			float _x = x * ca - y * sa;
			float _y = y * ca + x * sa;

			x = _x;
			y = _y;
		}

		void m90()
		{
			T tmp = x;
			x = y;
			y = -tmp;
		}

		Vector2<T> neg() const
		{
			return Vector2<T>(-x, -y);
		}

		T length() const
		{
			return sqrt(x * x + y * y);
		}

		T squareLength() const
		{
			return x * x + y * y;
		}

		void normalize()
		{
			T l = length();

			if (l > Traits::eps())
			{
				x /= l;
				y /= l;
			}
		}
	};


	typedef Vector2<float> Vector2f;
}
