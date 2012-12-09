#pragma once


namespace Construct
{
	template <class T>
	class Affine2d
	{
	public:
		enum
		{
			// col 0
			C0R0 = 0,
			C0R1,
			// col 1
			C1R0,
			C1R1,
			// col 2
			C2R0,
			C2R1,
			SIZE
		};

		// data
		T m[SIZE];


		Affine2d()
		{
			identity();
		}

		Affine2d(const Affine2d& other)
		{
			for (int i = 0; i < SIZE; ++i)
			{
				m[i] = other.m[i];
			}
		}

		Affine2d(T* t)
		{
			for (int i = 0; i < SIZE; ++i)
			{
				m[i] = t[i];
			}
		}

		Affine2d& operator=(const Affine2d& other)
		{
			for (int i = 0; i < SIZE; ++i)
			{
				m[i] = other.m[i];
			}

			return *this;
		}

		Affine2d mul(const Affine2d& other) const
		{
			Affine2d result;

			result.m[C0R0] = (m[C0R0] * other.m[C0R0]) + (m[C1R0] * other.m[C0R1]);
			result.m[C0R1] = (m[C0R1] * other.m[C0R0]) + (m[C1R1] * other.m[C0R1]);
			result.m[C1R0] = (m[C0R0] * other.m[C1R0]) + (m[C1R0] * other.m[C1R1]);
			result.m[C1R1] = (m[C0R1] * other.m[C1R0]) + (m[C1R1] * other.m[C1R1]);
			result.m[C2R0] = (m[C0R0] * other.m[C2R0]) + (m[C1R0] * other.m[C2R1]) + m[C2R0];
			result.m[C2R1] = (m[C0R1] * other.m[C2R0]) + (m[C1R1] * other.m[C2R1]) + m[C2R1];

			return result;
		}

		void transform(Vector2<T>& v) const
		{
			T x = (m[C0R0] * v.x) + (m[C1R0] * v.y) + m[C2R0];
			T y = (m[C0R1] * v.x) + (m[C1R1] * v.y) + m[C2R1];

			v.x = x;
			v.y = y;
		}

		void identity()
		{
			m[C0R0] = 1;
			m[C0R1] = 0;

			m[C1R0] = 0;
			m[C1R1] = 1;

			m[C2R0] = 0;
			m[C2R1] = 0;
		}

		void translation(T x, T y)
		{
			m[C0R0] = 1;
			m[C0R1] = 0;

			m[C1R0] = 0;
			m[C1R1] = 1;

			m[C2R0] = x;
			m[C2R1] = y;
		}

		Vector2<T> translation() const
		{
			return Vector2<T>(m[C2R0], m[C2R1]);
		}

		void scale(T xs, T ys)
		{
			m[C0R0] = xs;
			m[C0R1] = 0;

			m[C1R0] = 0;
			m[C1R1] = ys;

			m[C2R0] = 0;
			m[C2R1] = 0;
		}

		void rotation(float angleRad)
		{
			float ca = cos(angleRad);
			float sa = sin(angleRad);

			m[C0R0] = ca;
			m[C0R1] = sa;

			m[C1R0] = -sa;
			m[C1R1] = ca;

			m[C2R0] = 0;
			m[C2R1] = 0;
		}

		void create(float rotationAngleRad, T translationX, T translationY, T scaleX, T scaleY)
		{
			float ca = cos(rotationAngleRad);
			float sa = sin(rotationAngleRad);

			m[C0R0] = ca * scaleX;
			m[C0R1] = sa * scaleX;

			m[C1R0] = -sa * scaleY;
			m[C1R1] = ca * scaleY;

			m[C2R0] = translationX;
			m[C2R1] = translationY;
		}
	};

}
