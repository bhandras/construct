#pragma once


#include <algorithm>


struct Color4
{
	/// init with black, non transparent value
	Color4() : r(0.0f), g(0.0f), b(0.0f), a(1.0f) {}

	/// init with values from [0,1] interval
	Color4(float _r, float _g, float _b, float _a)
		: r(_r), g(_g), b(_b), a(_a) {}
	
	/// init with values from [0..255] range
	Color4(int _r, int _g, int _b, int _a)
		: r(static_cast<float>(_r) / 256.0f)
		, g(static_cast<float>(_g) / 256.0f)
		, b(static_cast<float>(_b) / 256.0f)
		, a(static_cast<float>(_a) / 256.0f) {}

	/// set with values from [0,1] interval
	void set(float _r, float _g, float _b, float _a)
	{
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}

	/// set with values from [0..255] range
	void set(int _r, int _g, int _b, int _a)
	{
		r = static_cast<float>(_r) / 256.0f;
		g = static_cast<float>(_g) / 256.0f;
		b = static_cast<float>(_b) / 256.0f;
		a = static_cast<float>(_a) / 256.0f;
	}

	void get(float* colorArray) const
	{
		colorArray[0] = r;
		colorArray[1] = g;
		colorArray[2] = b;
		colorArray[3] = a;
	}

	Color4 operator+(const Color4& other)
	{
		return Color4(r + other.r, g + other.g, b + other.b, a + other.a);
	}

	Color4& operator+=(const Color4& other)
	{
		r += other.r;
		g += other.g;
		b += other.b;
		a += other.a;

		return *this;
	}

	Color4 operator-(const Color4& other)
	{
		return Color4(r - other.r, g - other.g, b - other.b, a - other.a);
	}

	Color4& operator-=(const Color4& other)
	{
		r -= other.r;
		g -= other.g;
		b -= other.b;
		a -= other.a;

		return *this;
	}

	/// return color value with scaled components
	Color4 operator*(float scale)
	{
		return Color4(r * scale, g * scale, b * scale, a * scale);
	}

	/// scale *this
	Color4& operator*=(const float scale)
	{
		r *= scale;
		g *= scale;
		b *= scale;
		a *= scale;

		return *this;
	}

	Color4 interpolate(const Color4& other, float factor) const
	{
		Color4 result;

		result.r = factor * r + (1.0f - factor) * other.r;
		result.g = factor * g + (1.0f - factor) * other.g;
		result.b = factor * b + (1.0f - factor) * other.b;
		result.a = factor * a + (1.0f - factor) * other.a;

		return result;
	}

	/// normalize components to [0,1] interval
	void normalize()
	{
		r = std::max(0.0f, std::min(r, 1.0f));
		g = std::max(0.0f, std::min(g, 1.0f));
		b = std::max(0.0f, std::min(b, 1.0f));
		a = std::max(0.0f, std::min(a, 1.0f));
	}

	float r, g, b, a;
};
