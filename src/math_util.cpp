#include "construct.h"
// Perlin noise functions taken from this tutorial: http://freespace.virgin.net/hugo.elias/models/m_perlin.htm


namespace Construct
{
	int MathUtil::Random::seed = 0;


#define persistence 0.5f
#define Number_Of_Octaves 2


	void MathUtil::Random::setSeed(int newSeed)
	{
		seed = newSeed;
	}


	int MathUtil::Random::getInteger(int a, int b)
	{
		if (a > b)
		{
			int tmp = a;
			a = b;
			b = tmp;
		}

		seed = 214013 * seed + 2531011;
		return a + (seed ^ seed >> 15) % (b - a + 1);
	}


	float MathUtil::Random::getFloat(float a, float b)
	{
		if (a > b)
		{
			float tmp = a;
			a = b;
			b = tmp;
		}

		seed = 214013 * seed + 2531011;
		return a + (seed >> 16) * (1.0f / 65535.0f) * (b - a);
	}


	static float Noise_1(int x)
	{
		x = (x << 13)^x;
		return (1.0f - ((x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
	}


	static float SmoothedNoise_1(float x)
	{
		return Noise_1(static_cast<int>(x)) / 2.0f  +  Noise_1(static_cast<int>(x - 1)) / 4.0f  +  Noise_1(static_cast<int>(x + 1)) / 4.0f;
	}


	static float InterpplateCos(float a, float b, float x)
	{
		float ft = x * 3.1415927f;
		float f = (1.0f - cos(ft)) * 0.5f;

		return a  * (1.0f - f) + b * f;
	}


	static float InterpolatedNoise_1(float x)
	{
		int integer_X = static_cast<int>(x);
		float fractional_X = x - integer_X;

		float v1 = SmoothedNoise_1(static_cast<float>(integer_X));
		float v2 = SmoothedNoise_1(static_cast<float>(integer_X + 1));

		return InterpplateCos(v1 , v2 , fractional_X);
	}


	float MathUtil::PerlinNoise::perlinNoise1d(float x)
	{
		float total = 0;
		float p = persistence;
		int n = Number_Of_Octaves - 1;

		for (int i = 0; i < n; ++i)
		{
			float frequency = pow(2.0f, i);
			float amplitude = pow(p, i);

			total = total + InterpolatedNoise_1(x * frequency) * amplitude;
		}

		return total;
	}


	static float Noise_1(int x, int y)
	{
		int n = x + y * 57;
		n = (n << 13) ^ n;
		return (1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
	}


	static float SmoothedNoise_1(float x, float y)
	{
		float corners = (Noise_1(static_cast<int>(x - 1), static_cast<int>(y - 1)) +
			Noise_1(static_cast<int>(x + 1), static_cast<int>(y - 1)) +
			Noise_1(static_cast<int>(x - 1), static_cast<int>(y + 1)) +
			Noise_1(static_cast<int>(x + 1), static_cast<int>(y + 1))) / 16.0f;

		float sides = (Noise_1(static_cast<int>(x - 1), static_cast<int>(y)) +
			Noise_1(static_cast<int>(x + 1), static_cast<int>(y)) +
			Noise_1(static_cast<int>(x), static_cast<int>(y - 1)) +
			Noise_1(static_cast<int>(x), static_cast<int>(y + 1))) /  8.0f;

		float center =  Noise_1(static_cast<int>(x), static_cast<int>(y)) / 4.0f;

		return corners + sides + center;
	}


	static float InterpolatedNoise_1(float x, float y)
	{
		int integer_X = static_cast<int>(x);
		float fractional_X = x - integer_X;

		int integer_Y = static_cast<int>(y);
		float fractional_Y = y - integer_Y;

		float v1 = SmoothedNoise_1(static_cast<float>(integer_X), static_cast<float>(integer_Y));
		float v2 = SmoothedNoise_1(static_cast<float>(integer_X + 1), static_cast<float>(integer_Y));
		float v3 = SmoothedNoise_1(static_cast<float>(integer_X), static_cast<float>(integer_Y + 1));
		float v4 = SmoothedNoise_1(static_cast<float>(integer_X + 1), static_cast<float>(integer_Y + 1));

		float i1 = InterpplateCos(v1, v2, fractional_X);
		float i2 = InterpplateCos(v3, v4, fractional_X);

		return InterpplateCos(i1, i2, fractional_Y);
	}


	float MathUtil::PerlinNoise::perlinNoise2d(float x, float y)
	{
		float total = 0;
		// float p = persistence;
		int n = Number_Of_Octaves - 1;

		for (int i = 0; i < n; ++i)
		{
			float frequency = 0.25; //pow(2.0f, i);
			float amplitude = 1; //pow(p, i);

			total = total + InterpolatedNoise_1(x * frequency, y * frequency) * amplitude;
		}

		return total;
	}


	void MathUtil::Color::rgb2hsv(float r, float g, float b, float& h, float& s, float& v)
	{
		float min = std::min(r, std::min(g, b));
		float max = std::max(r, std::max(g, b));

		v = max;
		float delta = max - min;

		if (delta == 0)
		{
			h = -1;
			s = 0;
			v = max;
			return;
		}

		if( max != 0 )
		{
			s = delta / max;
		}
		else
		{
			// s = 0, v is undefined
			// r = g = b = 0
			s = 0;
			h = -1;
			return;
		}

		if (r == max)
		{
			// between yellow & magenta
			h = (g - b) / delta;
		}
		else if (g == max)
		{
			// between cyan & yellow
			h = 2 + (b - r) / delta;
		}
		else
		{
			// between magenta & cyan
			h = 4 + (r - g) / delta;
		}

		// degrees
		h *= 60;
		if (h < 0)
		{
			h += 360;
		}
	}


	void MathUtil::Color::hsv2rgb(float h, float s, float v, float& r, float& g, float& b)
	{
		if (s == 0)
		{
			// achromatic (grey)
			r = g = b = v;
			return;
		}

		// sector 0 to 5
		h /= 60;
		int i = static_cast<int>(h);

		// factorial part of h
		float f = h - i;
		float p = v * (1 - s);
		float q = v * (1 - s * f);
		float t = v * (1 - s * (1 - f));

		switch (i)
		{
		case 0:
			r = v;
			g = t;
			b = p;
			break;
		case 1:
			r = q;
			g = v;
			b = p;
			break;
		case 2:
			r = p;
			g = v;
			b = t;
			break;
		case 3:
			r = p;
			g = q;
			b = v;
			break;
		case 4:
			r = t;
			g = p;
			b = v;
			break;
		default:
			r = v;
			g = p;
			b = q;
			break;
		}
	}


	int MathUtil::Numeric::toInt(float num)
	{
		return NumericTraits<float>::toInt(num);
	}


	int MathUtil::Numeric::toInt(double num)
	{
		return NumericTraits<double>::toInt(num);
	}


	float MathUtil::Numeric::deg2Rad(float angleDeg)
	{
		return angleDeg * static_cast<float>(M_PI) / 180.0f;
	}

	float MathUtil::Numeric::rad2Deg(float angleRad)
	{
		return angleRad * 180.0f / static_cast<float>(M_PI);
	}

}
