#pragma once


namespace Construct
{
	template <class T>
	class Vector3
	{
	public:
		typedef NumericTraits<T> Traits;
		T x;
		T y;
		T z;

		Vector3()
			: x(static_cast<T>(0)), y(static_cast<T>(0)), z(static_cast<T>(0)) {}

		Vector3(const Vector3& other)
			: x(other.x), y(other.y), z(other.z) {}

		Vector3(T _x, T _y, T _z)
			: x(_x), y(_y), z(_z) {}

		void set(T _x, T _y, T _z)
		{
			x = _x;
			y = _y;
			z = _z;
		}

		Vector3& operator=(const Vector3& other)
		{
			x = other.x;
			y = other.y;
			z = other.z;

			return *this;
		}

		Vector3 operator+(const Vector3& other) const
		{
			return Vector3(x + other.x, y + other.y, z + other.z);
		}

		Vector3 operator-(const Vector3& other) const
		{
			return Vector3(x - other.x, y - other.y, z - other.z);
		}

		Vector3 operator*(float scalar) const
		{
			return Vector3(x * scalar, y * scalar, z * scalar);
		}

		T dot(const Vector3& other) const
		{
			return x * other.x + y * other.y + z * other.z;
		}

		Vector3 cross(const Vector3& other) const
		{
			return Vector3(
				y * other.z - z * other.y,
				z * other.x - x * other.z,
				x * other.y - y * other.x
				);
		}

		Vector3& operator+=(const Vector3& other)
		{
			x += other.x;
			y += other.y;
			z += other.z;

			return *this;
		}

		Vector3& operator-=(const Vector3& other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;

			return *this;
		}

		bool operator==(const Vector3& other) const
		{
			return x == other.x && y == other.y && z == other.z;
		}

		bool operator!=(const Vector3& other) const
		{
			return x != other.x || y != other.y || z != other.z;
		}

		void normalize()
		{
			T length = length();

			if (length > Traits::eps())
			{
				x /= length;
				y /= length;
				z /= length;
			}
		}

		T length() const
		{
			return sqrt(x * x + y * y + z * z);
		}
	};
}
