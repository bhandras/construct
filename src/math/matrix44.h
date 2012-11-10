#pragma once


template <class T>
class Matrix44
{
public:
	typedef NumericTraits<T> Traits;

	Matrix44()
	{
		for (int i = 0; i < 16; ++i)
		{
			mData[i] = Traits::zero();
		}
	}

	Matrix44(float a00, float a01, float a02, float a03,
		float a10, float a11, float a12, float a13,
		float a20, float a21, float a22, float a23,
		float a30, float a31, float a32, float a33)
	{
#define M(Y,X) m[(Y << 2) + X]

		M(0,0) = a00; M(0,1) = a01; M(0,2) = a02; M(0,3) = a03;
		M(1,0) = a10; M(1,1) = a11; M(1,2) = a12; M(1,3) = a13;
		M(2,0) = a20; M(2,1) = a21; M(2,2) = a22; M(2,3) = a23;
		M(3,0) = a30; M(3,1) = a31; M(3,2) = a32; M(3,3) = a33;

#undef M
	}

	Matrix44(const Matrix44& other)
	{
		for (int i = 0; i < 16; ++i)
		{
			mData[i] = other.mData[i];
		}
	}

	Matrix44(T *t)
	{
		for (int i = 0; i < 16; ++i)
		{
			mData[i] = t[i];
		}
	}

	Matrix44& operator=(const Matrix44& other)
	{
		for (int i = 0; i < 16; ++i)
		{
			mData[i] = other.mData[i];
		}

		return *this;
	}

	Matrix44& operator*=(const Matrix44& other)
	{
		*this = *this * other;
		return *this;
	}

	T& operator()(const int i, const int j)
	{
		return mData[(i << 2) + j];
	}

	T operator()(const int i, const int j) const
	{
		return mData[(i << 2) + j];
	}

	const T* data() const
	{
		return mData;
	}

	Matrix44 operator+(const Matrix44& other) const
	{
		Matrix44 result;

		for (int i = 0; i < 16; ++i)
		{
			result.mData[i] = mData[i] + other.mData[i];
		}

		return result;
	}


	Matrix44 operator-(const Matrix44& other) const
	{
		Matrix44 result;

		for (int i = 0; i < 16; ++i)
		{
			result.mData[i] = mData[i] - other.mData[i];
		}

		return result;
	}
	Matrix44 operator*(const Matrix44& other) const
	{
#define M1(Y,X) m[(Y << 2) + X]
#define M2(Y,X) other.m[(Y << 2) + X]
#define RESULT(Y,X) result.m[(Y << 2) + X]

		Matrix44 result;

		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				RESULT(i,j) = 
					M1(i,0) * M2(0,j) + 
					M1(i,1) * M2(1,j) + 
					M1(i,2) * M2(2,j) +
					M1(i,3) * M2(3,j);
			}
		}

		return result;

#undef M1
#undef M2
#undef RESULT
	}

	Vector3<T> operator*(const Vector3<T>& v) const
	{
#define M(X,Y) m[(Y << 2) + X]

		T x = v.x();
		T y = v.y();
		T z = v.z();

		return Vector3<T>(
			M(0,0)*x + M(1,0)*y + M(2,0)*z + M(3,0),
			M(0,1)*x + M(1,1)*y + M(2,1)*z + M(3,1),
			M(0,2)*x + M(1,2)*y + M(2,2)*z + M(3,2)
		);
#undef M
	}

	Matrix44 operator*(float scalar) const
	{
		Matrix44 result;

		for (int i = 0; i < 16; ++i)
		{
			result.mData[i] = mData[i] * scalar;
		}

		return result;
	}

	void identity()
	{
#define M(Y,X) mData[(Y << 2) + X]

		for (int i = 0; i < 16; ++i)
		{
			mData[i] = Traits::zero();
		}
		M(0,0) = M(1,1) = M(2,2) = M(3,3) = Traits::one();

#undef M
	}

	static Matrix44 TranslationMatrix(const Vector3<T>& translation)
	{
		Matrix44 result;
		result.identity();

#define M(Y,X) result.mData[(Y << 2) + X]

		M(3,0) = translation.z();
		M(3,1) = translation.y();
		M(3,2) = translation.z();

#undef M

		return result;
	}

	static Matrix44 ScalingMatrix(const Vector3<T>& scale)
	{
		Matrix44 Result;
		Result.identity();

#define M(Y,X) Result.mData[(Y << 2) + X]

		M(0,0) = scale.x();
		M(1,1) = scale.y();
		M(2,2) = scale.z();

#undef M

		return Result;
	}

	static Matrix44 RotationMatrix(float angle, const Vector3<T>& axis)
	{
		float cosAngle = cos(angle);
		float sinAngle = sin(angle);

		float a = 1.0f - cosAngle;
		float ax = a * axis.x();
		float ay = a * axis.y();
		float az = a * axis.z();

		Matrix44 result;
		result.identity();

		result.mData[0] = ax * axis.x() + cosAngle;
		result.mData[1] = ax * axis.y() - axis.z() * sinAngle;
		result.mData[2] = ax * axis.z() + axis.y() * sinAngle;

		result.mData[4] = ay * axis.x() + axis.z() * sinAngle;
		result.mData[5] = ay * axis.y() + cosAngle;
		result.mData[6] = ay * axis.z() - axis.x() * sinAngle;

		result.mData[8]  = az * axis.x() - axis.y() * sinAngle;
		result.mData[9]  = az * axis.y() + axis.x() * sinAngle;
		result.mData[10] = az * axis.z() + cosAngle;

		return result; 
	}

private:
	T mData[16];
};
